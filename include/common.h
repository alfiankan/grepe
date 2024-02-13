#ifndef HEADER_COMMON_H
#define HEADER_COMMON_H




#define NEW_LINE_SEPARATOR '\n'
#define TERM_COLOR_DEFAULT "\033[0m"
#define TERM_COLOR_GRAY "\033[1;30m"
#define TERM_COLOR_RED "\033[1;31m"
#define TERM_COLOR_GREEN "\033[1;32m"
#define MAX_GROUP_PATTERN 10

#define LEAP_IS_HOUR 1
#define LEAP_IS_MINUTE 2
#define LEAP_IS_SECOND 3
#define LEAP_IS_DAY 4
#define LEAP_UNDEFINED 0


struct LabelBucket {
  char *key;
  int value;
};

struct TSBucket {
  struct TSBucket *next_bucket;
  long long ts_start;
  long long ts_end;
  struct LabelBucket *label_bucket;
};

struct LegendCharMap {
  char *key;
  char value;
};

struct DecoderResult {
  struct TSBucket *head; 
  int max;
  int min;
  int total_scanned;
  int group_pattern_total;
  struct LegendCharMap *legend;
  char **group_by; 
};


int get_os_page_size(); 

long long convert_string_to_time_millis(char* str_time, char* format); 

int regexp_find_match(char *pattern, char *text_data, int max_char_matches, char *match_result); 

int get_terminal_width();

int time_ms_to_formated_string_date_time(long long time_ms, char *datetime_format, char *formated_result);

struct tm *time_ms_to_tm(long long time_ms); 

int step_leap(int step); 

#endif


