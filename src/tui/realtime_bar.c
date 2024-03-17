#include <curses.h>
#include <signal.h>
#include <stdlib.h>
//
//
//
struct TuiCell {
  char chr;
  int color_pair;
};

typedef struct TuiCell t_cell;
//
//
//struct UiThreadArgs {
//  int mapwinx;
//  int mapwiny;
//  struct TuiCell *tui_cell_buffer[MAX_X][MAX_Y];
//};
//
//
//static void *run_slide_tui(void *arg) {
//    struct UiThreadArgs *ui_args = (struct UiThreadArgs *)arg;
//
//    ui_args->tui_cell_buffer[10][10]->chr;
//
//    exit(0);
//
////    for (int y = 0; y < ui_args->mapwiny; y++) {
////        for (int x = 0; x < ui_args->mapwinx; x++) {
////            struct TuiCell *cell = bubu[y][x];
////            printf("%c", cell->chr);
////        }
////        printf("\n");
////    }
////    exit(0);
//    /*
//  initscr();
//  cbreak();
//  noecho();
//  keypad(stdscr, TRUE);
//  refresh();
//
//  WINDOW *mapwin;
//
//
//  exit(0);
//
//
//  mapwin = newwin(ui_args->mapwiny, ui_args->mapwinx, 0, 0);
//
//  while (true) {
//    int x, y;
//    for (x = 0; x < ui_args->mapwinx; x++) {
//      for (y = 0; y < ui_args->mapwiny; y++) {
//        if (ui_buffer[x][y].color_pair > -1) {
//          mvwaddch(mapwin, y, x, ui_buffer[x][y].chr | COLOR_PAIR(ui_buffer[x][y].color_pair));
//        } else {
//          mvwaddch(mapwin, y, x, ui_buffer[x][y].chr);
//        }
//      }
//    }
//    wrefresh(mapwin);
//  }
//
//
//  endwin();
//  */
//
//}
