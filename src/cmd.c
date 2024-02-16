#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "./decoder/line_decoder.c"

int main(int argc, char *argv[]) {
    int opt;

    /*
  grepe --time-pattern "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}" \
        --time-format "%d/%b/%Y:%H:%M:%S" \
        --start "2024-02-11 22:17:00" \
        --end "2024-02-11 22:18:00" \
        --step 10 \
        --group-by '200' '400' \
        --path mylog
     * */

    char *time_start = "2023-11-04 12:30:00";
    char *time_end = "2023-11-04 13:00:00";
    int window_step = 60 * 5;
    char *source_path = "mylog";
    char *time_pattern = "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}";
    char *time_format = "%d/%b/%Y:%H:%M:%S";
    char *group_by_pattern[10] = {};
    int group_total = 0;
    char *symbol = "▓";

    static struct option long_options[] = {
        {"group-by", 1, NULL, 'g'},
        {"time-pattern", 1, NULL, 'z'},
        {"time-format", 1, NULL, 'f'},
        {"start", 1, NULL, 's'},
        {"end", 1, NULL, 'e'},
        {"step", 1, NULL, 'w'},
        {"path", 1, NULL, 'k'},
    };

    while ((opt = getopt_long(argc, argv, "z:f:s:e:w:k:g", long_options, NULL)) != -1) {
        switch (opt) {
            case 'z':
                time_pattern = strdup(optarg);
                break;
            case 'f':
                time_format = optarg;
                break;
            case 's':
                time_start = strdup(optarg);
                break;
            case 'e':
                time_end = strdup(optarg);
                break;
            case 'w':
                window_step = atoi(optarg);
                break;
            case 'k':
                source_path = strdup(optarg);
                break;
            case 'g':
        
                optind--;
                for( ;optind < argc && *argv[optind] != '-'; optind++){
                  group_by_pattern[group_total] = strdup(argv[optind]);
                  group_total += 1;
                }

                
                break;
            case ':':
                fprintf(stderr, "Option requires an argument.\n");
                return EXIT_FAILURE;
            case '?':
                fprintf(stderr, "Unknown option: '%c'\n", optopt);
                return EXIT_FAILURE;
        }
    }

  printf("====== This Is Beta Version, create Issue here https://github.com/alfiankan/grepe/issues ======\n");
  /*
  printf("time pattern %s \n", time_pattern);
  printf("time format %s \n", time_format);
  printf("time start %s \n", time_start);
  printf("time end %s \n", time_end);
  printf("time step %d \n", window_step);
  printf("file path %s \n", source_path);
  printf("total group pattern %d \n", group_total);
  for (int x=0; x < group_total; x++) {
    printf("group by %s \n", group_by_pattern[x]);
  }
  */


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
