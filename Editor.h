//
// Created by sk on 7/20/21.
//

#ifndef TEXT_EDITOR_EDITOR_H
#define TEXT_EDITOR_EDITOR_H

#include <ncurses.h>
#include "Buffer.h"


class Editor {

private:
    int x, y;

    char mode;
    Buffer* buffer;

    std::string status, filename;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void deleteLine();
    void deleteLine(int);

    void saveFile();

public:
    Editor();
    explicit Editor(const std::string&);

    char getMode() const
    {
      return mode;
    }

    void handInput(int);
    void printBuffer();
    void printStatusLine();
    void updateStatus();
};


#endif //TEXT_EDITOR_EDITOR_H
