#include <ncurses.h>
#include <string>
#include <vector>
#include "Editor.h"


void curses_init()
{
  initscr();                      // Start ncurses mode
  noecho();                       // Don't echo keystrokes
  cbreak();                       // Disable line buffering
  keypad(stdscr, true);           // Enable special keys to be recorded
  start_color();

  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);

  attron(COLOR_PAIR(1));
  attron(COLOR_PAIR(2));
  refresh();
}

int main(int argc, char* argv[])
{
  std::string fn;
  Editor ed;

  if(argc > 1)
  {
    fn = argv[1];               // Set the filename
    ed = Editor(fn);
  }
  else
  {
    ed = Editor();
  }

  curses_init();                  // Initialize ncurses
  while(ed.getMode() != 'x')
  {
    ed.updateStatus();
    ed.printStatusLine();
    ed.printBuffer();

    int input = getch();
    ed.handInput(input);
  }

  refresh();                      // Refresh display
  endwin();                       // End ncurses mode
  return 0;
}