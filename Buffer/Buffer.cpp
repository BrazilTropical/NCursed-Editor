//
// Created by sk on 7/20/21.
//

#include "Buffer.h"

Buffer::Buffer() = default;

std::string Buffer::remove_tabs(std::string line)
{
  std::size_t tab = line.find('\t');

  if(tab == line.npos)
    return line;

  return remove_tabs(line.replace(tab, 1, ">>"));
}

void Buffer::insert_line(std::string line, int n)
{
  line = remove_tabs(line);

  Buffer::lines.insert(Buffer::lines.begin() + n, line);
}

void Buffer::append_line(std::string line)
{
  line = remove_tabs(line);

  Buffer::lines.push_back(line);
}

void Buffer::remove_line(int n)
{
  Buffer::lines.erase(Buffer::lines.begin() + n);
}
