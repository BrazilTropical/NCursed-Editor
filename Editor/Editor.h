//
// Created by sk on 7/20/21.
//

#ifndef TEXT_EDITOR_EDITOR_H
#define TEXT_EDITOR_EDITOR_H

#include <ncurses.h>
#include "../Buffer/Buffer.h"
#include "Cursor.h"
#include <string>

#define BACKGROUND_COLOR 1
#define TEXT_COLOR 2

class Editor {

private:
    Cursor cursor {0, 0};
    int rows, columns;
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

    void set_columns(const int& col) { this->columns = col; }
    void set_rows(const int& row) { this->rows = row; }

    void handle_input(int);
    void print_buffer();
    void print_status_line();
    void update_status();
};


#endif //TEXT_EDITOR_EDITOR_H
