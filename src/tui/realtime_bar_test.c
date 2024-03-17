#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct TuiCell {
    char chr;
    int color_pair;
};
typedef struct TuiCell t_cell;



struct UiThreadArgs {
  int mapwinx;
  int mapwiny;
  int some_number;
  t_cell *tui_cell_buffer;
};


static void *printbuffer(void *arg) {
    struct UiThreadArgs *ui_args = (struct UiThreadArgs*) arg;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();

  WINDOW *mapwin;
  mapwin = newwin(ui_args->mapwiny, ui_args->mapwinx, 0, 0);

  for (;;) {
      for (int x = 0; x < ui_args->mapwinx; ++x) {
          for (int y = 0; y < ui_args->mapwiny; ++y) {
            t_cell cell = ui_args->tui_cell_buffer[x * ui_args->mapwiny + y];
            if (cell.color_pair > -1) {
                mvwaddch(mapwin, y, x, cell.chr | COLOR_PAIR(cell.color_pair));
            } else {
                mvwaddch(mapwin, y, x, cell.chr);
            }
          }
      }
      wrefresh(mapwin);

  }
}

static void *counter(void *args) {
    struct UiThreadArgs *ui_args = (struct UiThreadArgs*) args;

    for (int y = 0; y < ui_args->mapwiny; ++y) {
        for (int x = 0; x < ui_args->mapwiny; ++x) {
            t_cell *cell = &ui_args->tui_cell_buffer[x * ui_args->mapwiny + y];
            cell->chr = '#';
            sleep(1);
        }
    }
}

int main() {
  int mapwinx, mapwiny;

  initscr();
  getmaxyx(stdscr, mapwiny, mapwinx);
  endwin();
  printf("x=%d | y=%d\n", mapwinx, mapwiny);


  t_cell *ptr = malloc((mapwinx * mapwiny) * sizeof (t_cell));

  printf("Size off struct %lu byte\n", sizeof (t_cell));

  for (int i = 0; i < (mapwiny * mapwinx); ++i) {
      struct TuiCell cell = {
              .chr = '.',
              .color_pair = -1
      };
      ptr[i] = cell;
  }

  struct UiThreadArgs ui_args = {
          .mapwinx = mapwinx,
          .mapwiny = mapwiny,
          .some_number = 0,
          .tui_cell_buffer = ptr
  };

  pthread_t ui_thread;
  pthread_t counter_thread;
  pthread_create(&ui_thread, NULL, printbuffer, &ui_args);
  pthread_create(&counter_thread, NULL, counter, &ui_args);
  pthread_join(ui_thread, NULL);
  pthread_join(counter_thread, NULL);

  printbuffer(&ui_args);

  free(ptr);

  return 0;
}
