#include <stdio.h>
#include "line_decoder.c"
//#include "horizontal_bar.c"

int main() {

  char *time_start = "2024-02-11 22:16:00";
  char *time_end = "2024-02-11 22:17:00";
  int window_step = 60;
  char *source_path = "mylog";
  char *time_pattern = "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}";
  char *time_format = "%d/%b/%Y:%H:%M:%S";
  char *group_by_pattern[] = {"200", "400", "Mozilla"};
  int group_total = 3;

  struct TSBucket ts_bucket = { 
    .ts_start = 0, 
    .ts_end = 0, 
    .label_bucket = NULL, 
    .next_bucket = NULL,
  };

  struct LegendCharMap *legend_map = NULL;

  struct DecoderResult decoded_result = {
    .max = 0,
    .min = 0,
    .total_scanned = 0,
    .head = &ts_bucket,
    .legend = legend_map,
    .group_pattern_total = group_total,
  };
  decoded_result.group_by = group_by_pattern;

  if (process_time_series_text_line(
   time_pattern,
    time_format,
    time_start,
    time_end,
    window_step,
    source_path,
    group_by_pattern,
    group_total,
    &decoded_result
  ) != 0) {
    perror("Failed to process data");
    return 1;
  }

  // display
  //print_time_series_horizontal_bar(&decoded_result);


}
