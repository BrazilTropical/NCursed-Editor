//
// Created by sk on 7/20/21.
//

#include "Buffer.h"

#include "Buffer.h"

Buffer::Buffer() = default;

std::string Buffer::remTabs(std::string line)
{
  int tab = line.find("\t");

  if(tab == line.npos)
    return line;

  return remTabs(line.replace(tab, 1, "  "));
}

void Buffer::insertLine(std::string line, int n)
{
  line = remTabs(line);

  Buffer::lines.insert(Buffer::lines.begin() + n, line);
}

void Buffer::appendLine(std::string line)
{
  line = remTabs(line);

  Buffer::lines.push_back(line);
}

void Buffer::removeLine(int n)
{
  Buffer::lines.erase(Buffer::lines.begin() + n);
}
