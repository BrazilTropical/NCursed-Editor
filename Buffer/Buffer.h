//
// Created by sk on 7/20/21.
//

#ifndef TEXT_EDITOR_BUFFER_H
#define TEXT_EDITOR_BUFFER_H

#include <string>
#include <vector>

class Buffer
{
public:
    Buffer();

    std::vector<std::string> lines;

    void insertLine(std::string, int);
    void appendLine(std::string);
    void removeLine(int);

    std::string remTabs(std::string);
};

#endif //TEXT_EDITOR_BUFFER_H
