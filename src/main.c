#include <stdio.h>
#define STB_DS_IMPLEMENTATION
#include "../include/stb_ds.h"
#include <time.h>
#include <regex.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>



long long convert_string_to_time_millis(char* str_time, char* format) {
  struct tm time_start_struct;
  strptime(str_time, format, &time_start_struct);
  time_start_struct.tm_isdst = -1; 
  time_t timemillis_start = mktime(&time_start_struct);
  return timemillis_start;
}

int regexp_find_match(char *pattern, char *text_data, int max_char_matches, char *match_result) {
  regex_t rg;
  regmatch_t rg_result[max_char_matches];
  
  int reg_comp_result = regcomp(&rg, pattern, REG_EXTENDED);
  if (reg_comp_result != 0) {
    char error_message[max_char_matches];
    regerror(reg_comp_result, &rg, error_message, sizeof(error_message));
    fprintf(stderr, "Failed to compile regex pattern: %s\n", error_message);
    return reg_comp_result;
  }

  //char found_matches[max_char_matches];

  int result = regexec(&rg, text_data, max_char_matches, rg_result, 0);

  if (result == 0) {
    
    int len = rg_result[0].rm_eo - rg_result[0].rm_so;
    if (len < max_char_matches - 1) {
      strncpy(match_result, text_data + rg_result[0].rm_so, len);
      //found_matches[len] = '\0';
      //printf("REGEX FOUND MATCHES: %s\n", match_result);

      return 0;
    } else {
      fprintf(stderr, "Matched portion is too long\n");
    }

  } else if(result == REG_NOMATCH) {
    //printf("NO MATCH FOR %s \n", pattern);
    return result;
  } else {
    char error_message[100];
    regerror(result, &rg, error_message, sizeof(error_message));
    fprintf(stderr, "Failed to execute regex: %s\n", error_message);
    return result;
  }
  return -1;
}

int time_ms_to_formated_string_date_time(long long time_ms, char *datetime_format, char *formated_result) {
  time_t unix_time = (time_t) (time_ms / 1000LL);
  struct tm *tm_info;

  tm_info = localtime(&unix_time);

  int res =strftime(formated_result, 20, datetime_format, tm_info);
  if (res != 0) {
    return res;
  }

  return 0;
}

int main(int argc, char *argv[]) {

  const char *decoder = argv[1];
  char *time_format = argv[2];
  char *time_pattern = argv[3];
  char *time_start = argv[4];
  char *time_end = argv[5];
  char *window_step = argv[6];
  char *group_by = argv[7];
  const char *source_path = argv[8];

  const char* log = "203.0.113.10 - - [11/Feb/2024:12:34:56 +0000] \"GET /index.html HTTP/1.1\" 200 1024 \"-\" \"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.190 Safari/537.36\"";
  printf("%s \n", log);

  char* log_time_pattern = "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}";

  // bind test
  time_start = "2024-01-01 07:07:00";
  time_end = "2024-01-01 08:08:25";
  window_step = "10";
  source_path = "mylog";
  time_pattern = "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}";
  time_format = "%d/%b/%Y:%H:%M:%S";
  group_by = "401";
  
  int window_step_range = atoi(window_step);
 
  int *t_series_size;
  int *t_series;

  long long step = (long long) window_step_range;
 
  // read file line perline
  int fd = open(source_path, O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  struct stat sb;

  fstat(fd, &sb);


  int page_size = (int) sysconf(_SC_PAGESIZE);
  if (page_size == -1) {
      perror("sysconf");
      return 1;
  }

  printf("System memory page size: %d bytes\n", page_size);

  int total_pages = (int) ((sb.st_size + page_size - 1) / page_size);

  int log_scanned_total = 0;


  // linkedlist

  struct TSBucket {
    struct TSBucket *next_bucket;
    long long ts_start;
    long long ts_end;
    int total;
  };

  struct TSBucket ts_bucket_head = { .ts_start = 0, .ts_end = 0, .total = 0, .next_bucket = NULL };

  struct TSBucket *current_ts_bucket_node = &ts_bucket_head;
  printf("HEADDDD=%p\n", current_ts_bucket_node);

  long long very_limit_time = 1707660960000;

  for (int page_num = 0; page_num < total_pages; page_num++) {
  

    char *mapped;
    char *contents;

    mapped = mmap(NULL, page_size, PROT_READ, MAP_PRIVATE, fd, page_num * page_size);


    int scan_pointer_position = 0;
    for (;;) {
      // find size of line so we can allocate
      int line_size = 0;
      int inline_scan_pointer = scan_pointer_position;
      
      int is_end_of_page = 0;
      for (;;) {
        if (inline_scan_pointer + 1 > page_size) {
          //printf("END SCAN POSITION IS %d size=%d \n", inline_scan_pointer, line_size); 
          is_end_of_page = 1;
          break;
        }

        if (mapped[inline_scan_pointer] == '\n') {
          line_size += 1;
          break;
        }
        line_size += 1;
        inline_scan_pointer += 1;
      } 

      if (is_end_of_page) {
        break;
      }


      char *log_line = malloc(line_size);
      // actual copy
      for (int x = 0; x < line_size; x++) {
        log_line[x] = mapped[scan_pointer_position + x];
      } 
      //printf("%s \n", log_line);


      // TODO: process here
      char *time_log_part = malloc(line_size);
      if (regexp_find_match(time_pattern, log_line, line_size, time_log_part) == REG_NOMATCH) {
        //printf("NOMATCHING ========");
        log_scanned_total += 1;

        free(log_line);

        scan_pointer_position += line_size;
        continue;
      }

      long long log_time_ms = convert_string_to_time_millis(time_log_part, time_format) * 1000LL;

      if (current_ts_bucket_node->ts_start == 0) {
        // init time series window

        current_ts_bucket_node->ts_start = log_time_ms;
        current_ts_bucket_node->ts_end = current_ts_bucket_node->ts_start + (step * 1000LL);

        // init first node
        

      }

      // check if current log time is betwenn time step window
      //printf("CHECK IN WINDOW %lld <= %lld <= %lld \n", t_series_start, log_time_ms, t_series_end);
      if ((current_ts_bucket_node->ts_start <= log_time_ms && log_time_ms <= current_ts_bucket_node->ts_end) == 1) {
        // count group
        //printf("COUNTING\n");

        if (regexp_find_match(group_by, log_line, line_size, time_log_part) == 0) {
          current_ts_bucket_node->total += 1;  
        }


      } else {
        // move window and recheck log_time
        //printf("OUTSIDE\n");

        for (;;) {
          
          // new node and connect check if log_time inside window break
          struct TSBucket *next_node = (struct TSBucket *)malloc(sizeof(struct TSBucket));

          
          next_node->total = 0;
          next_node->ts_start = current_ts_bucket_node->ts_end;
          next_node->ts_end = current_ts_bucket_node->ts_end + (step * 1000LL);
          next_node->next_bucket = NULL;
          

          // connect set current
          current_ts_bucket_node->next_bucket = next_node;
          current_ts_bucket_node = next_node;

          if ((current_ts_bucket_node->ts_start <= log_time_ms && log_time_ms <= current_ts_bucket_node->ts_end) == 1) {
            // please break
            if (regexp_find_match(group_by, log_line, line_size, time_log_part) == 0) {
              current_ts_bucket_node->total += 1;  
            }
            break;
          }

        }


      }


      log_scanned_total += 1;

      free(log_line);

      scan_pointer_position += line_size;

    }
    if (munmap(mapped, page_size) == -1) {
      perror("munmap");
      return 1;
    }
  }

  close(fd);

  printf("total log scanned %d \n", log_scanned_total);


  // print all nodes
  struct TSBucket *curr_tbucket =  &ts_bucket_head;

  for (;;) {


    char time_x_legend[20];
    time_ms_to_formated_string_date_time(curr_tbucket->ts_start, "%Y-%m-%d %H:%M:%S", time_x_legend);

    printf("[%s] total=%d ", time_x_legend, curr_tbucket->total );

    // ■■■■■■■
    // ■■■■■■■

    for (int x = 0; x < curr_tbucket->total;x++) {
      printf("■");
    }
    printf("\n");

    if (curr_tbucket->next_bucket == NULL) {
      break;
    }
    //curr_tbucket = curr_tbucket->next_bucket; 
    curr_tbucket = curr_tbucket->next_bucket;
      
    

  }

  return 0;
}
