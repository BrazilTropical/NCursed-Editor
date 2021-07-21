//
// Created by sk on 7/20/21.
//

#ifndef TEXT_EDITOR_EDITOR_H
#define TEXT_EDITOR_EDITOR_H

#include <ncurses.h>
#include "../Buffer/Buffer.h"
#include <string>

#define BACKGROUND_COLOR 1
#define TEXT_COLOR 2

struct Cursor
{
    int x;
    int y;
};

class Editor {

private:
    Cursor cursor {0, 0};
    int start_print_index = 0;
    int line_numbering_offset = 0;
    bool is_modified;

    char mode;
    Buffer* buffer;

    std::string status, filename;

    void move_up();
    void move_down();
    void move_left();
    void move_right();

    void move_back_word();
    void move_word();

    void delete_line();
    void delete_line(int);

    void save_file();

public:
    Editor();
    explicit Editor(const std::string&);

    char get_mode() const { return mode; }

    void handle_input(int);
    void print_buffer();
    void print_status_line();
    void update_status();
};


#endif //TEXT_EDITOR_EDITOR_H
