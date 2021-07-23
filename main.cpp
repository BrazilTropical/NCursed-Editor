#include <ncurses.h>
#include <string>
#include <vector>
#include "Editor/Editor.h"

void curses_init()
{
  initscr();                      // Start ncurses mode
  noecho();                       // Don't echo keystrokes
  raw();
  cbreak();                       // Disable line buffering
  keypad(stdscr, true);           // Enable special keys to be recorded

  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);


  refresh();
}

int main(int argc, char* argv[])
{
  Editor ed;

  ed = argc > 1 ? Editor(argv[1]) : Editor();

  curses_init();                  // Initialize ncurses

  while(ed.get_mode() != 'x')     // Loop until close
  {
    ed.update_status();
    ed.print_status_line();
    ed.print_buffer();

    refresh();
    ed.handle_input(getch());
  }

  refresh();                      // Refresh display
  endwin();                       // End ncurses mode
  return 0;
}