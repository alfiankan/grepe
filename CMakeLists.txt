cmake_minimum_required( VERSION 3.0 )

project( grepe )


include_directories( ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/libs)
add_executable( 
  grepe-test 
  ${CMAKE_SOURCE_DIR}/src/common.c
  ${CMAKE_SOURCE_DIR}/src/tests/test.c
)

add_executable( 
  grepe 
  ${CMAKE_SOURCE_DIR}/src/common.c
  ${CMAKE_SOURCE_DIR}/src/cmd.c
)



#add_subdirectory( ./libs/cJSON )
#target_link_libraries( grepe cjson )




