cmake_minimum_required( VERSION 3.0 )

project( grepe )

include_directories( ./include ./src ./libs)
add_executable( grepe ./src/main.c )


#add_subdirectory( ./libs/cJSON )
#target_link_libraries( grepe cjson )




