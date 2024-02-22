#include "../include/common.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <regex.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define NEW_LINE_SEPARATOR '\n'
#define TERM_COLOR_DEFAULT "\033[0m"
#define TERM_COLOR_GRAY "\033[1;30m"
#define TERM_COLOR_RED "\033[1;31m"
#define TERM_COLOR_GREEN "\033[1;32m"
#define MAX_GROUP_PATTERN 10



int get_os_page_size() {
  struct winsize ws;
  if ( 
    ioctl( STDIN_FILENO , TIOCGWINSZ, &ws ) != 0 &&
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &ws ) != 0 &&
    ioctl( STDERR_FILENO, TIOCGWINSZ, &ws ) != 0 
  ) {
    return 500;
  }
  return ws.ws_col;

}

long long convert_string_to_time_millis(char* str_time, char* format) {
  struct tm time_start_struct;
  strptime(str_time, format, &time_start_struct);
  time_start_struct.tm_isdst = -1; 
  time_t timemillis_start = mktime(&time_start_struct);
  return timemillis_start * 1000LL;
}


int regexp_find_match(char *pattern, char *text_data, int max_char_matches, char *match_result) {
  max_char_matches += 50;
  regex_t rg;
  regmatch_t rg_result[max_char_matches];
  
  int reg_comp_result = regcomp(&rg, pattern, REG_EXTENDED);
  if (reg_comp_result != 0) {
    char error_message[max_char_matches];
    regerror(reg_comp_result, &rg, error_message, sizeof(error_message));
    fprintf(stderr, "Failed to compile regex pattern: %s\n", error_message);
    regfree(&rg);
    return reg_comp_result;
  }

  int result = regexec(&rg, text_data, max_char_matches, rg_result, 0);

  if (result == 0) {
    regfree(&rg); 
    int len = rg_result[0].rm_eo - rg_result[0].rm_so;
    if (len < max_char_matches - 1) {
      strncpy(match_result, text_data + rg_result[0].rm_so, len);
      return 0;
    } else {
      fprintf(stderr, "Matched portion is too long %d from %d char\n", len, max_char_matches - 1);
      return 1;
    }

  } else if(result == REG_NOMATCH) {
    regfree(&rg);
    return 1;
  } else {
    char error_message[100];
    regerror(result, &rg, error_message, sizeof(error_message));
    fprintf(stderr, "Failed to execute regex: %s\n", error_message);
    regfree(&rg);
    return 1;
  }
  return 1;
}

int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}


int time_ms_to_formated_string_date_time(long long time_ms, char *datetime_format, char *formated_result) {
  time_t unix_time = (time_t) (time_ms / 1000LL);
  struct tm *tm_info;

  tm_info = localtime(&unix_time);
  tm_info->tm_isdst = -1;

  strftime(formated_result, 50, datetime_format, tm_info);

  return 0;
}


struct tm *time_ms_to_tm(long long time_ms) {
  time_t unix_time = (time_t) (time_ms / 1000LL);
  struct tm *tm_info;

  tm_info = localtime(&unix_time);
  tm_info->tm_isdst = -1;

  return tm_info;
}


#define LEAP_IS_HOUR 1
#define LEAP_IS_MINUTE 2
#define LEAP_IS_SECOND 3
#define LEAP_IS_DAY 4
#define LEAP_UNDEFINED 0

int step_leap(int step) {

  if ((step / 60) == 0) {
    return LEAP_IS_SECOND;
  } else if ((step / 60) >= 1 && (step / 60) < 60 ) {
    return  LEAP_IS_MINUTE;
  } else if ( (step/60) >= 60 && (step / 60) < 1440) {
    return LEAP_IS_HOUR;
  } else if ( (step/60) >= 1440 ) { 
    return LEAP_IS_DAY;
  } else {
    return LEAP_UNDEFINED;
  }

}
