#include <ncurses.h>
#include <string>
#include <vector>
#include "Editor/Editor.h"

void curses_init()
{
  initscr();                      // Start ncurses mode
  noecho();                       // Don't echo keystrokes
  cbreak();                       // Disable line buffering
  keypad(stdscr, true);           // Enable special keys to be recorded

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