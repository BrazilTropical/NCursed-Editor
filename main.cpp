#include <ncurses.h>
#include <string>
#include <vector>
#include "Editor/Editor.h"

void curses_init(Editor ed)
{
  initscr();                      // Start ncurses mode
  noecho();                       // Don't echo keystrokes
  raw();
  cbreak();                       // Disable line buffering
  keypad(stdscr, true);           // Enable special keys to be recorded

  int rows, columns;
  getmaxyx(stdscr, rows, columns);
  ed.set_columns(columns);
  ed.set_rows(rows);

  scrollok(stdscr, TRUE);

  refresh();
}

int main(int argc, char* argv[])
{
  Editor ed;

  ed = argc > 1 ? Editor(argv[1]) : Editor();

  curses_init(ed);                  // Initialize ncurses

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