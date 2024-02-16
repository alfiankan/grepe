#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../../include/common.h"
#include <string.h>
#define STB_DS_IMPLEMENTATION
#ifndef HEADER_STB_H
#define HEADER_STB_H
#include "../../libs/stb/stb_ds.h"
#endif


int print_header(
  char *time_start, 
  char *time_end,
  struct DecoderResult *decoded_head 
) {
  int term_width = get_terminal_width(); 
  for (int x = 0; x < term_width; x++) {
    printf("=");
  }
  printf("\n");


  printf("  %s \n", "grepe v0.1.0-beta");

  printf("  TIME RANGE  : %s ~ %s \n", time_start, time_end);

  printf("  PROCESSED   : %d line \n", decoded_head->total_scanned);

  printf("  MAX         : %d \n", decoded_head->max);
  printf("  MIN         : %d \n\n", decoded_head->min);


  printf("  Legend      : \n");
  for (int group_key_n = 0; group_key_n < decoded_head->group_pattern_total; group_key_n++) {
    char legend = stbds_hmget(decoded_head->legend, decoded_head->group_by[group_key_n]);
    printf("  [%c] %s \n", legend, decoded_head->group_by[group_key_n]);
  }


  for (int x = 0; x < term_width; x++) {
    printf("=");
  }
  printf("\n");

  return 0;
}
