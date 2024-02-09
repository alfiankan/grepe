cmake_minimum_required( VERSION 3.0 )

project( grepe )

include_directories( ./src ./libs/cJSON)

add_subdirectory( ./libs/cJSON )


add_executable( grepe ./src/main.c )

target_link_libraries( grepe cjson )


