#include <stdio.h>
#include <stdlib.h>


int generate_nginx_default_log(char *log_path) {
  printf("Starting Generating Logs\n");

  FILE *file = fopen(log_path, "a"); 
  if (file == NULL) {
      perror("Error opening file");
      return 1;
  }


  fprintf(file, "203.0.113.10 - - [%s +0000] 'GET /index.html HTTP/1.1' %s 1024 '-' 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.190 Safari/537.36'\n", "200", "200");

  fclose(file);

  return 0;
}
