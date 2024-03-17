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

add_executable( 
  grepe-demo-gen-log 
  ${CMAKE_SOURCE_DIR}/src/common.c
  ${CMAKE_SOURCE_DIR}/src/gen_log_cmd.c
)

add_executable( 
  tui-test 
  ${CMAKE_SOURCE_DIR}/src/tui/realtime_bar_test.c
)

target_link_libraries(tui-test ncurses)


install(TARGETS grepe DESTINATION bin)
install(TARGETS grepe-demo-gen-log DESTINATION bin)





