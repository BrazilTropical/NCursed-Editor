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
  cursor.x = 0, cursor.y = 0;
  rows = 0, columns = 0;
  is_modified = false;
  mode ='n';

  status = "Normal Mode";
  filename = "Untitled";

  buffer = new Buffer();
  buffer->append_line("");
}

Editor::Editor(const std::string& filename)
{
  cursor.x = 0, cursor.y = 0;
  rows = 0, columns = 0;
  is_modified = false;
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
      buffer->append_line(temp);
    }
  }
  else
  {
    std::cerr << "Cannot open file '" << filename << "'\n";
    buffer->append_line("");
  }
}

void Editor::update_status()
{
  std::string a;
  a = std::to_string(LINES);
  switch (mode)
  {
    case 'n': status = "Normal Mode " + a; break;
    case 'i': status = "Insert Mode"; break;
    case 'x': status = "Exiting";     break;
  }

  status += "\t" + std::to_string(cursor.x) + ":" + std::to_string(cursor.y);
}

void Editor::handle_input(int c)
{
  switch (c)
  {
    case KEY_LEFT:
      move_left();
      return;
    case KEY_RIGHT:
      move_right();
      return;
    case KEY_UP:
      move_up();
      return;
    case KEY_DOWN:
      move_down();
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
        case 's':
          save_file(); break;

        //vim-style navigation
        case 'h':
        case 'H':
          move_left(); break;

        case 'j':
        case 'J':
          move_down(); break;

        case 'k':
        case 'K':
          move_up();   break;

        case 'l':
        case 'L':
          move_right(); break;

        case 'b':
          move_back_word(); break;
        case 'w':
          move_word(); break;
      }
      break;

    case 'i':
      switch (c)
      {
        case KEY_ESCAPE: mode = 'n'; break;

        case KEY_BACKSPACE:
        case KEY_BACKSPACE2:
          if(!cursor.x && cursor.y > 0)
          {
            cursor.x = buffer->lines[cursor.y - 1].length();
            //bring line down
            buffer->lines[cursor.y - 1] += buffer->lines[cursor.y];
            delete_line();
            move_up();
          }
          else if(cursor.x)
          {
            buffer->lines[cursor.y].erase(--cursor.x, 1);
          }
          break;

        case KEY_DC:
          if(cursor.x == buffer->lines[cursor.y].length() && cursor.y != buffer->lines.size() - 1)
          {
            buffer->lines[cursor.y] += buffer->lines[cursor.y + 1];
            delete_line((int) cursor.y + 1);
          }
          else
          {
            buffer->lines[cursor.y].erase(cursor.x, 1);
          }
          break;

        case KEY_ENTER:
        case KEY_LINEFEED:
          if(cursor.x < buffer->lines[cursor.y].length())
          {
            //make a new line with the content followed by the cursor
            buffer->insert_line(buffer->lines[cursor.y].substr(cursor.x, buffer->lines[cursor.y].length() - cursor.x),
                                (int) cursor.y + 1);
            //remove that part of the line
            buffer->lines[cursor.y].erase(cursor.x, buffer->lines[cursor.y].length() - cursor.x);
          }
          else
          {
            buffer->insert_line("", (int) cursor.y + 1);
          }

          cursor.x = 0;
          move_down();
          break;

        case KEY_BTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case KEY_CTAB:
        case KEY_HORIZONTALTAB:
          buffer->lines[cursor.y].insert(cursor.x, TAB_SIZE, ' ');
          cursor.x += TAB_SIZE;
          break;

        default:
          buffer->lines[cursor.y].insert(cursor.x, 1, char(c));
          cursor.x++;
          break;
      }
      break;
  }
}

void Editor::move_left()
{
  if(cursor.x - 1 >= 0)
  {
    cursor.x--;
    move((int) cursor.y, (int) cursor.x);
  }
  else if(cursor.y > 0)
  {
    cursor.y--;
    cursor.x = buffer->lines[cursor.y].length();
    move((int) cursor.y, (int) cursor.x);
  }

}

void Editor::move_right()
{
  if(cursor.x + 1 <= buffer->lines[cursor.y].length())
  {
    cursor.x++;
    move((int) cursor.y, (int) cursor.x);
  }
  else if(cursor.x + 1 > buffer->lines[cursor.y].length() && cursor.y + 1 < buffer->lines.size() - 1)
  {
    cursor.x = 0;
    cursor.y++;
    move((int) cursor.y, (int) cursor.x);
  }
}

void Editor::move_up()
{
  if(cursor.y - 1 >= 0)
    cursor.y--;

  if(cursor.x >= buffer->lines[cursor.y].length())
    cursor.x = buffer->lines[cursor.y].length();

  move((int) cursor.y, (int) cursor.x);
}

void Editor::move_down()
{
  if(cursor.y + 1 <= buffer->lines.size())
    cursor.y++;

  if(cursor.x >= buffer->lines[cursor.y].length())
    cursor.x = buffer->lines[cursor.y].length();

  move((int) cursor.y, (int) cursor.x);
  refresh();
}

//Moves to previous encounter of SPACE
//Vim does something similar but it moves to the first encounter of a non alpha character
//It's fine by now...
void Editor::move_back_word()
{
  std::size_t prev_space_x = buffer->lines[cursor.y].find_last_of(' ', cursor.x);

  if(prev_space_x <= 0 || (prev_space_x > cursor.x && cursor.y > 0))
    cursor.y--;

  cursor.x = prev_space_x <= 0 ? buffer->lines[cursor.y].size() : prev_space_x - 1; //move to end of line or prev word
}

//Same as above, but we get the next encounter of SPACE
void Editor::move_word()
{
  std::size_t next_space_x = buffer->lines[cursor.y].find_first_of(' ', cursor.x);

  if(next_space_x < cursor.x && cursor.y < LINES - 1)
  {
    cursor.y++;
  }

  cursor.x = next_space_x + 1;
}

void Editor::print_buffer()
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

  move((int) cursor.y, (int) cursor.x);
}

void Editor::print_status_line()
{
  attron(A_REVERSE);
  mvprintw(LINES - 1, 0, status.c_str());
  clrtoeol();
  attroff(A_REVERSE);
}

void Editor::delete_line()
{
  buffer->remove_line((int) cursor.y);
}

void Editor::delete_line(int i)
{
  buffer->remove_line(i);
}

void Editor::save_file()
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

    is_modified = false;
  }
  else
  {
    status = "Error: Cannot open file for writing!";
  }


  f.close();
}