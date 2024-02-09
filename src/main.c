#include <stdio.h>
#include "../libs/cJSON/cJSON.h"


int main(int argc, char *argv[]) {

  const char* source_path = argv[1];

  const char* data = "[1,2]";

  cJSON *json = cJSON_Parse(data);


  printf("Hello, World! %s %p \n", source_path, json);
  return 0;
}
