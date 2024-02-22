#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../include/common.h"

int generate_nginx_default_log(char *log_path) {
  printf("Starting Generating Logs\n");

  int generated = 0;
  int target = 1024 * 1024 * 5; // 1GB 1073741824

  FILE *file = fopen(log_path, "a"); 
  if (file == NULL) {
      perror("Error opening file");
      return 1;
  }


  long long current_time = 1704042000000;


  current_time -= 86400LL * 1000LL * 60LL;

  int status_codes[] = {200, 201, 400, 404, 502};
  int num_status_codes = sizeof(status_codes) / sizeof(int);

  

  for (;;) {
    if (generated > target) {
      break;
    }

    // random 200 to 700 ms
    long long random_number = (long long) (200 + (rand() % (700 - 200 + 1)));
    current_time += random_number;

    int random_status_code = status_codes[rand() % num_status_codes];

    char buffer_time[64];
    time_ms_to_formated_string_date_time(current_time, "%d/%b/%Y:%H:%M:%M", buffer_time);
    generated += fprintf(file, "203.0.113.10 - - [%s +0000] 'GET /index.html HTTP/1.1' %d 1024 '-' 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.190 Safari/537.36'\n", buffer_time, random_status_code);

  }
  fclose(file);

  printf("Finished Generating %d bytes Logs\n", generated);

  return 0;
}
