//
// Created by sk on 7/20/21.
//

#ifndef TEXT_EDITOR_BUFFER_H
#define TEXT_EDITOR_BUFFER_H

#include <string>
#include <vector>

#define TAB_SIZE 2

class Buffer
{
public:
    Buffer();

    std::vector<std::string> lines;

    void insert_line(std::string, int);
    void append_line(std::string);
    void remove_line(int);

    std::string remove_tabs(std::string);
};

#endif //TEXT_EDITOR_BUFFER_H
