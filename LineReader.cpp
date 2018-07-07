#include "LineReader.h"

#include <fstream>
#include <string>
#include <stdexcept>

LineReader::LineReader(const char *file_name)
        : f(file_name, std::ifstream::binary)
{
    if (!f) {
        throw std::runtime_error("Не получилоcь открыть для чтения файл " + std::string(file_name));
    }
}

const char * LineReader::next_line()
{
    std::ifstream::pos_type pos = f.tellg();
    line_pos.push_back(pos);
    if (!getline(f, buff) || buff.empty()) {
        f.clear();
        return NULL;
    }
    delete_trailing_cr();
    return buff.c_str();
}

#include <iostream>

const char * LineReader::read_line(unsigned line_num)
{
    f.seekg(line_pos[line_num]);
    getline(f, buff);
    delete_trailing_cr();
    return buff.c_str();
}

void LineReader::delete_trailing_cr()
{
    auto it = buff.end() - 1;
    if (*it == '\r') {
        buff.erase(it);
    }
}
