#include <stdio.h>
#include "../decoder/line_decoder.c"

int main() {


  // test
  char *time_start = "2024-02-10 22:16:00";
  char *time_end = "2024-02-12 22:17:00";
  int window_step = 60;
  char *source_path = "mylog";
  char *time_pattern = "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}";
  char *time_format = "%d/%b/%Y:%H:%M:%S";
  char *group_by_pattern[] = {"200", "400", "Mozilla"};
  int group_total = 3;
  char *symbol = "▓";

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
