#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <regex.h>
#include <time.h>
#include "../../include/common.h"
#include "../tui/horizontal_bar.c"
#include "../tui/head.c"
#include <errno.h>
#include <math.h>
#include <unistd.h>

#define STB_DS_IMPLEMENTATION

#ifndef HEADER_STB_H
#define HEADER_STB_H
#include "../../libs/stb/stb_ds.h"
#endif

/*
 * parsing line text using regex
 * */
int process_time_series_text_line(
  char *time_pattern, 
  char *time_format, 
  char *ts_start_datetime, 
  char *ts_end_datetime, 
  int step, 
  char *path, 
  char *group_by[],
  int group_pattern_total,
  struct DecoderResult *decoded
) {

  // open file descriptor
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  // get file stat
  struct stat source_file_stat;
  if (fstat(fd, &source_file_stat) != 0) {
    perror("failed to stat file");
    return EXIT_FAILURE;
  }

  int os_page_size = get_os_page_size() * 300;


  int log_scanned_total = 0;

  struct TSBucket *current_ts_bucket_node = decoded->head;

  long long ts_cmd_start_ms = convert_string_to_time_millis(ts_start_datetime, "%Y-%m-%d %H:%M:%S");
  long long ts_cmd_end_ms = convert_string_to_time_millis(ts_end_datetime, "%Y-%m-%d %H:%M:%S");

  int total_pages = (int) ((source_file_stat.st_size + os_page_size) / os_page_size);
  printf("TOTAL PAGES SIZE: %d \n", os_page_size);
  // iterate over page

  char *carry_page_data = malloc(10);
  int total_carry_bytes = 0;
  int total_bytes_scanned = 0;


  for (int page_num = 0; page_num < total_pages; page_num++) {
    // map to ram
    //printf("Start Scanning page: %d from offset %d [page_size: %d]\n", page_num, page_num * os_page_size, os_page_size);

    char *mapped_chunk = mmap(
      NULL, 
      os_page_size, 
      PROT_READ, 
      MAP_PRIVATE, 
      fd, 
      page_num * os_page_size
    );
    if (mapped_chunk == MAP_FAILED) {
      break;
    }

    // to align last page
    int page_drift = (source_file_stat.st_size - total_bytes_scanned) - os_page_size;
    if (page_drift >= 0) {
      page_drift = 0;
    }

    int in_page_scan_cursor_position = 0;
    int last_in_line_offset_end = 0;
    for (;;) {
      int line_size = 0;
      int in_line_scan_cursor_position = in_page_scan_cursor_position;
      int is_end_of_page = 0;

      // LINE SCANNING
      for (;;) {


        // check drift allignment
        
        if (page_drift != 0) {
          if (in_line_scan_cursor_position > (os_page_size + page_drift)) {
            is_end_of_page = 1;
            break;
          }
        }
        

        total_bytes_scanned += 1;
        if (in_line_scan_cursor_position + 1 > os_page_size) {
          // break here to continue to next page
          // but if log line is in 2 page we set read offset with length scanned
          
          //read_offset += last_in_line_offset_end;
          //printf("reset page_size %d - %d\n", os_page_size, os_page_size - last_in_line_offset_end);

          unsigned long carry_total_bytes = (unsigned long) (os_page_size - last_in_line_offset_end);
          if (carry_total_bytes > 0) {
            // we need to carry overflow offset data
            // realloc size
            carry_page_data = realloc(carry_page_data, carry_total_bytes);

            char *buffer_from_offset = mapped_chunk + (last_in_line_offset_end );
            memcpy(carry_page_data, buffer_from_offset, carry_total_bytes);
            total_carry_bytes = carry_total_bytes;
            
            //printf("\n======\nCARRIED (%s) SIZE %ld last scanned offset %d\n======\n", carry_page_data, carry_total_bytes, last_in_line_offset_end);
          }

          //exit(1);

          is_end_of_page = 1;
          break;
        }

        // check if EOL break to one line to next further process
        //printf("(%d)", mapped_chunk[in_line_scan_cursor_position]); 
        if (mapped_chunk[in_line_scan_cursor_position] == '\n') {
          last_in_line_offset_end = in_line_scan_cursor_position;
          line_size += 1;
          // scan nextline
          break;
        }
        line_size += 1;
        in_line_scan_cursor_position += 1;
      }

      // outer early break
      if (is_end_of_page == 1) {
        break;
      }

      
      char *log_line = malloc(line_size + total_carry_bytes);
      //printf("LINE SIZE %ld \n", sizeof(char));
      // copy line
      // TODO: refactor use memcp

      // check if carry page data exist read first and append to logline
      int write_alignment_with_carry_data = 0;
      if (total_carry_bytes > 0) {
        for (int x = 0; x < total_carry_bytes; x++) {
          log_line[x] = carry_page_data[x]; 
        } 

        write_alignment_with_carry_data = total_carry_bytes;

        // reset carry data
        total_carry_bytes = 0;
      }

      for (int x = 0; x < line_size; x++) {
        //printf("SCANNED %c \n", mapped_chunk[in_line_scan_cursor_position + x]);
        log_line[x + write_alignment_with_carry_data] = mapped_chunk[in_page_scan_cursor_position + x];
      }

      
      //printf("===== (%d) \n", log_scanned_total);
     

      if (log_scanned_total == 1000) {
        //exit(1);
      }

      //printf("LOG alignment (%d) [%d] : %s total size %d at page: %d \n", write_alignment_with_carry_data, log_scanned_total, log_line, line_size, page_num); 

      // break time part by using regex
      char *time_log_part = malloc(50);

      int matching_time_result = regexp_find_match(time_pattern, log_line, line_size + write_alignment_with_carry_data + 50, time_log_part);

      if ( matching_time_result != 0) {
        // non match time format pattern
        log_scanned_total += 1;
        in_page_scan_cursor_position += line_size;

        printf("Failed to parse time at line (%d): %s \n", log_scanned_total, log_line);
        //exit(1); 

        //printf("LOG alignment (%d) [%d] : (%s) total size %d at page: %d \n", write_alignment_with_carry_data, log_scanned_total, log_line, line_size, page_num); 
        free(time_log_part);
        free(log_line);

        continue;
      }
      //exit(1);
 
      long long log_time_ms = convert_string_to_time_millis(time_log_part, time_format);
      // check logline is inside time window from defined cmd flags
      
      
      if ((ts_cmd_start_ms <= log_time_ms && log_time_ms <= ts_cmd_end_ms) == 0) {
        log_scanned_total += 1;

        free(time_log_part);
        free(log_line);

        in_page_scan_cursor_position += line_size;
        continue;
      }

      // check if node was empty set time between
      if (current_ts_bucket_node->ts_start == 0 && current_ts_bucket_node->next_bucket == NULL) {

        // normalize to hour 00:00:00
        struct tm *first_time_tm = time_ms_to_tm(log_time_ms);

        int leap = step_leap(step);
        switch (leap) {
        case LEAP_IS_SECOND:
          first_time_tm->tm_sec = 0;
          break;
        case LEAP_IS_MINUTE:
          first_time_tm->tm_sec = 0;
          break;
        case LEAP_IS_HOUR:
          first_time_tm->tm_sec = 0;
          first_time_tm->tm_min = 0;
          break;
        case LEAP_IS_DAY:
          first_time_tm->tm_hour = 0;
          first_time_tm->tm_sec = 0;
          first_time_tm->tm_min = 0;
          break;
        default:
          break;
        }

        char *normalized_start_datetime = malloc(20 * sizeof(char));
        sprintf(
          normalized_start_datetime, 
          "%d-%02d-%02d %02d:%02d:%02d\n",
          1900 + first_time_tm->tm_year,
          first_time_tm->tm_mon + 1,
          first_time_tm->tm_mday, 
          first_time_tm->tm_hour, 
          first_time_tm->tm_min,
          first_time_tm->tm_sec
        );
        current_ts_bucket_node->ts_start = convert_string_to_time_millis(normalized_start_datetime, "%Y-%m-%d %H:%M:%S");
        free(normalized_start_datetime); 
        current_ts_bucket_node->ts_end = current_ts_bucket_node->ts_start + (step * 1000LL);
      }



      // check if log line inside step window update the node
      if ((current_ts_bucket_node->ts_start <= log_time_ms && log_time_ms <= current_ts_bucket_node->ts_end) == 1) {
        
        // iterate over group keys
        for (int x = 0; x < group_pattern_total; x++) {
          if (regexp_find_match(group_by[x], log_line, line_size, time_log_part) == 0) {
            int curr = stbds_hmget(current_ts_bucket_node->label_bucket, group_by[x]);
            stbds_hmput(current_ts_bucket_node->label_bucket, group_by[x], curr + 1);
          } 
        }

      } else {
        // move window extend node
        for (;;) {
          struct TSBucket *next_node = (struct TSBucket *)malloc(sizeof(struct TSBucket));
          next_node->label_bucket = NULL;
          next_node->ts_start = current_ts_bucket_node->ts_end;
          next_node->ts_end = current_ts_bucket_node->ts_end + (step * 1000LL);
          next_node->next_bucket = NULL;

          // connect set current
          current_ts_bucket_node->next_bucket = next_node;
          current_ts_bucket_node = next_node;
            
          if ((current_ts_bucket_node->ts_start <= log_time_ms && log_time_ms <= current_ts_bucket_node->ts_end) == 1) {

            for (int x = 0; x < group_pattern_total;x++) {
              if (regexp_find_match(group_by[x], log_line, line_size, time_log_part) == 0) {
                int curr = stbds_hmget(current_ts_bucket_node->label_bucket, group_by[x]);
                stbds_hmput(current_ts_bucket_node->label_bucket, group_by[x], curr + 13);
              }
            }
            break;
          } 
        }
      }
      log_scanned_total += 1;
      free(time_log_part);
      free(log_line);
      in_page_scan_cursor_position += line_size;
    }





    // unmap
    if (munmap(mapped_chunk, os_page_size) == -1) {
      perror("munmap");
      return EXIT_FAILURE;
    }

    // print info scanned page

    printf("\r");
    printf("Processing [%.2f%%] %d/%d bytes", ( (double) total_bytes_scanned / (double) source_file_stat.st_size)*100, total_bytes_scanned, (int) source_file_stat.st_size);
    
    //break;
    if (total_bytes_scanned >= source_file_stat.st_size) {
      break;
    }
    fflush(stdout);
  }
  printf("\n");

  free(carry_page_data);

  close(fd);

  // mapping LEGEND
  char legend_alphabet[MAX_GROUP_PATTERN] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
  int legend_alphabet_used_offset = 0;

  for (int x = 0; x < group_pattern_total; x++) {
    stbds_hmput(decoded->legend, group_by[x], legend_alphabet[legend_alphabet_used_offset]);
    legend_alphabet_used_offset += 1;
  }


  // find max min
  
  int max_height = 0;
  int min_height = 0;
  struct TSBucket *curr_tbucket =  decoded->head;

  for (;;) {
    for (int x = 0; x < group_pattern_total; x++) {

      int actual_total = stbds_hmget(curr_tbucket->label_bucket, group_by[x]);
      
      if (actual_total > max_height) {
        max_height = actual_total;
      }
      if (min_height == 0) {
        min_height = actual_total;
      }
      if (actual_total < min_height) {
        min_height = actual_total;
      }
    }
    if (curr_tbucket->next_bucket == NULL) {
      break;
    }
    curr_tbucket = curr_tbucket->next_bucket; 
  }

  decoded->max = max_height;
  decoded->min = min_height;
  decoded->total_scanned = log_scanned_total;

  return EXIT_SUCCESS;
}



int run_time_series_for_horizontal_bar(
  char *time_pattern, 
  char *time_format, 
  char *ts_start_datetime, 
  char *ts_end_datetime, 
  int step, 
  char *path, 
  char *group_by[],
  int group_pattern_total,
  char *symbol
) {
  struct LabelBucket *label_bucket_map = NULL;


  struct TSBucket ts_bucket = { 
    .ts_start = 0, 
    .ts_end = 0, 
    .label_bucket = label_bucket_map, 
    .next_bucket = NULL,
  };

  struct LegendCharMap *legend_map = NULL;

  struct DecoderResult decoded_result = {
    .max = 0,
    .min = 0,
    .total_scanned = 0,
    .head = &ts_bucket,
    .legend = legend_map,
    .group_pattern_total = group_pattern_total,
  };
  decoded_result.group_by = group_by;
   

  if (process_time_series_text_line(
    time_pattern,
    time_format,
    ts_start_datetime,
    ts_end_datetime,
    step,
    path,
    group_by,
    group_pattern_total,
    &decoded_result
  ) != 0) {
    perror("Failed to process data");
    return 1;
  }
  // display
  print_header(ts_start_datetime, ts_end_datetime, &decoded_result);

  print_time_series_horizontal_bar(&decoded_result, symbol);

  return 0;
}


