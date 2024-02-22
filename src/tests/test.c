#include <stdio.h>
#include "../decoder/line_decoder.c"
#include "../tools/nginx_log_generator.c"

int main() {


  // test
  char *time_start = "2020-11-04 12:30:00";
  char *time_end = "2029-11-04 13:00:00";
  int window_step = 30;
  char *source_path = "mylog";
  char *time_pattern = "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}";
  char *time_format = "%d/%b/%Y:%H:%M:%S";
  char *group_by_pattern[] = {"200", "400"};
  int group_total = 2;
  char *symbol = "▓";
 

  // remove test file
  remove(source_path);

  generate_nginx_default_log(source_path); 

  run_time_series_for_horizontal_bar(
    time_pattern,
    time_format,
    time_start,
    time_end,
    window_step,
    source_path,
    group_by_pattern,
    group_total,
    symbol
  );


  return 0;

}
