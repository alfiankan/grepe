#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>



int main(int argc, char *argv[]) {
    int opt;

    /*
     
  time_start = "2024-02-11 22:17:00";
  time_end = "2024-02-11 22:18:00";
  window_step = "10";
  source_path = "mylog";
  time_pattern = "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}";
  time_format = "%d/%b/%Y:%H:%M:%S";
  group_by = "200";
  char *bar_symbol = "▓";

  grepe --time-pattern "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}" \
        --time-format "%d/%b/%Y:%H:%M:%S" \
        --start "2024-02-11 22:17:00" \
        --end "2024-02-11 22:18:00" \
        --step 10 \
        --group-by '200' \
        --group-by '400' \
        --path mylog

     * */

    static struct option long_options[] = {
        {"time-pattern", 1, NULL, 'p'},
        {"time-format", 1, NULL, 'f'},
        {"start", 1, NULL, 's'},
        {"end", 1, NULL, 'e'},
        {"step", 1, NULL, 'w'},
        {"path", 1, NULL, 'k'},
        {"group-by", 1, NULL, 'g'},
    };

    while ((opt = getopt_long(argc, argv, "p:f:s:e:w:k:g", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                printf("time pattern: %s\n", optarg);
                break;
            case 'f':
                printf("time format: %s\n", optarg);
                break;
            case 's':
                printf("start: %s\n", optarg);
                break;
            case 'e':
                printf("end: %s\n", optarg);
                break;
            case 'w':
                printf("step: %s\n", optarg);
                break;
            case 'k':
                printf("path: %s\n", optarg);
                break;
            case 'g':
                printf("group by: %s\n", optarg);
                break;
            case ':':
                fprintf(stderr, "Option requires an argument.\n");
                return EXIT_FAILURE;
            case '?':
                fprintf(stderr, "Unknown option: '%c'\n", optopt);
                return EXIT_FAILURE;
        }
    }

    

    return 0;
}
