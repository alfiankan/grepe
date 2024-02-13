#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../include/common.h"
#include <string.h>
#define STB_DS_IMPLEMENTATION
#include "../include/stb_ds.h"

int print_time_series_horizontal_bar(struct DecoderResult *decoded) {

  int term_width = get_terminal_width();

  char *bar_symbol = "▓";

  struct TSBucket *curr_tbucket = decoded->head;


  for (;;) {

    printf("%c", NEW_LINE_SEPARATOR);
    
    for (int group_key_n = 0; group_key_n < decoded->group_pattern_total; group_key_n++) {

      // convert millis to readable date time
      char time_x_legend[20];
      time_ms_to_formated_string_date_time(curr_tbucket->ts_start, "%Y-%m-%d %H:%M:%S", time_x_legend);

      char *line_display = malloc(sizeof(char) * term_width);
      int actual_total = stbds_hmget(curr_tbucket->label_bucket, decoded->group_by[group_key_n]);

      int total_on_bar_max_size = snprintf(NULL, 0, " (%d)", actual_total) + 2;
      char *total_on_bar = malloc(sizeof(char) * total_on_bar_max_size);
      sprintf(total_on_bar, " (%d)", actual_total);

      // strcuture
      // 20 chars date
      // 1 space
      // 2 char sub group
      // 1 space
      // n total length
      // 1 space
      // 10 char total
      
      char *legend_alphabet_alias = malloc(sizeof(char) * 3);
      legend_alphabet_alias[0] = ' ';

      legend_alphabet_alias[1] = stbds_hmget(decoded->legend, decoded->group_by[group_key_n]);
      legend_alphabet_alias[2] = ' ';

      strcat(line_display, time_x_legend);
      strcat(line_display, " | ");
      strcat(line_display, legend_alphabet_alias);

      // char left
      int char_left = term_width - (20+1+2+1+total_on_bar_max_size);

      int bar_normalized_delta = (decoded->max / char_left);
      // if < 0 so reduce by delta to fit the terminal

      int display_bar_length = (actual_total / bar_normalized_delta);
      if (bar_normalized_delta <= 0) {
        display_bar_length = actual_total;
      }

      // repaint
      if (actual_total >= decoded->max) {
        strcat(line_display, TERM_COLOR_RED); 
      } else if (actual_total <= decoded->min) {
        strcat(line_display, TERM_COLOR_GREEN); 
      } else {
        strcat(line_display, TERM_COLOR_GRAY);
      }

      // draw bar
      for (int x=0;x<display_bar_length;x++) {
        strcat(line_display, bar_symbol);
      }
      strcat(line_display, TERM_COLOR_DEFAULT);
      strcat(line_display, total_on_bar);
      printf("%s", line_display);
      printf("\n");

      // add line separator if more than one legend
      /*
      if (((int) stbds_hmlenu(curr_tbucket->label_bucket)) > 1 ) {
        for (int x = 0; x < term_width; x++) {
          printf("_");
        }
        printf("\n");
      }*/
    }
    // next node
    if (curr_tbucket->next_bucket == NULL) {
      break;
    }
    curr_tbucket = curr_tbucket->next_bucket;
  }


  return 0;

}


