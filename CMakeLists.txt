cmake_minimum_required( VERSION 3.0 )

project( grepe )


include_directories( ${CMAKE_SOURCE_DIR}/include)
add_executable( grepe ${CMAKE_SOURCE_DIR}/src/cmd.c )
add_executable( 
  grepe-test 
  ${CMAKE_SOURCE_DIR}/src/test.c 
  ${CMAKE_SOURCE_DIR}/src/common.c
  ${CMAKE_SOURCE_DIR}/src/line_decoder.c
  ${CMAKE_SOURCE_DIR}/src/horizontal_bar.c
)



#add_subdirectory( ./libs/cJSON )
#target_link_libraries( grepe cjson )




