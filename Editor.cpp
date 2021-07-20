//
// Created by sk on 7/20/21.
//

#include "Editor.h"

#include <fstream>
#include <iostream>

#define KEY_ESCAPE 27
#define KEY_BACKSPACE2 127
#define KEY_LINEFEED 10
#define KEY_HORIZONTALTAB 9

Editor::Editor()
{
  x = 0, y = 0;
  mode ='n';

  status = "Normal Mode";
  filename = "Untitled";

  buffer = new Buffer();
  buffer->appendLine("");
}

Editor::Editor(const std::string& filename)
{
  x = 0;
  y = 0;
  mode = 'n';

  this->filename = filename;

  buffer = new Buffer();

  std::ifstream infile(filename.c_str());

  if(infile.is_open())
  {
    while(!infile.eof())
    {
      std::string temp;
      getline(infile, temp);
      buffer->appendLine(temp);
    }
  }
  else
  {
    std::cerr << "Cannot open file '" << filename << "'\n";
    buffer->appendLine("");
  }
}

void Editor::updateStatus()
{
  switch (mode)
  {
    case 'n': status = "Normal Mode"; break;
    case 'i': status = "Insert Mode"; break;
    case 'x': status = "Exiting";     break;
  }

  status += "\t" + std::to_string(x) + ":" + std::to_string(y);
}

void Editor::handInput(int c)
{
  switch (c)
  {
    case KEY_LEFT:
      moveLeft();
      return;
    case KEY_RIGHT:
      moveRight();
      return;
    case KEY_UP:
      moveUp();
      return;
    case KEY_DOWN:
      moveDown();
      return;
    default:
      break;
  }

  switch (mode)
  {
    case 'n':
      switch (c)
      {
        case 'x': mode = 'x'; break;
        case 'i': mode = 'i'; break;
        case 's': saveFile(); break;

        //vim-style navigation
        case 'h':
        case 'H': moveLeft(); break;

        case 'j':
        case 'J': moveDown(); break;

        case 'k':
        case 'K': moveUp();   break;

        case 'l':
        case 'L': moveRight(); break;
      }
      break;

    case 'i':
      switch (c)
      {
        case KEY_ESCAPE: mode = 'n'; break;

        case KEY_BACKSPACE:
        case KEY_BACKSPACE2:
          if(!x && y > 0)
          {
            x = buffer->lines[y - 1].length();
            //bring line down
            buffer->lines[y - 1] += buffer->lines[y];
            deleteLine();
            moveUp();
          }
          else if(x)
          {
            buffer->lines[y].erase(--x, 1);
          }
          break;

        case KEY_DC:
          if(x == buffer->lines[y].length() && y != buffer->lines.size() - 1)
          {
            buffer->lines[y] += buffer->lines[y + 1];
            deleteLine(y + 1);
          }
          else
          {
            buffer->lines[y].erase(x, 1);
          }
          break;

        case KEY_ENTER:
        case KEY_LINEFEED:
          if(x < buffer->lines[y].length())
          {
            //make a new line with the content followed by the cursor
            buffer->insertLine(buffer->lines[y].substr(x, buffer->lines[y].length() - x), y + 1);
            //remove that part of the line
            buffer->lines[y].erase(x, buffer->lines[y].length() - x);
          }
          else
          {
            buffer->insertLine("", y + 1);
          }
          x = 0;
          moveDown();
          break;

        case KEY_BTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case KEY_CTAB:
        case KEY_HORIZONTALTAB:
          buffer->lines[y].insert(x, 4, ' ');
          x += 4;
          break;

        default:
          buffer->lines[y].insert(x, 1, char(c));
          x++;
          break;
      }
      break;
  }
}

void Editor::moveLeft()
{
  if(x - 1 >= 0)
  {
    x--;
    move(y, x);
  }
}

void Editor::moveRight()
{
  if(x + 1 < COLS && x + 1 <= buffer->lines[y].length())
  {
    x++;
    move(y, x);
  }
}

void Editor::moveUp()
{
  if(y - 1 >= 0)
    y--;

  if(x >= buffer->lines[y].length())
    x = buffer->lines[y].length();

  move(y, x);
}

void Editor::moveDown()
{
  if(y + 1 < LINES - 1 && y + 1 < buffer->lines.size())
    y++;

  if(x >= buffer->lines[y].length())
    x = buffer->lines[y].length();

  move(y, x);
}

void Editor::printBuffer()
{
  for(int i = 0; i < LINES - 1; ++i)
  {
    if(i >= buffer->lines.size())
    {
      move(i, 0);
      clrtoeol();
    }
    else
    {
      mvprintw(i, 0, buffer->lines[i].c_str());
    }
    clrtoeol();
  }

  move(y, x);
}

void Editor::printStatusLine()
{
  attron(A_REVERSE);
  mvprintw(LINES - 1, 0, status.c_str());
  clrtoeol();
  attroff(A_REVERSE);
}

void Editor::deleteLine()
{
  buffer->removeLine(y);
}

void Editor::deleteLine(int i)
{
  buffer->removeLine(i);
}

void Editor::saveFile()
{
  if(filename.empty())
  {
    filename = "untitled";
  }

  std::ofstream f(filename.c_str());

  if(f.is_open())
  {
    for(auto & line : buffer->lines)
      f << line << std::endl;

    status = "Saved to file!";
  }
  else
  {
    status = "Error: Cannot open file for writing!";
  }

  f.close();
}