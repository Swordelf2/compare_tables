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
    if (cur_line == line_pos.size()) {
        std::ifstream::pos_type pos = f.tellg();
        line_pos.push_back(pos);
    }
    ++cur_line;
    if (!getline(f, buff)) {
        f.clear();
        return NULL;
    }
    delete_trailing_cr();
    return buff.c_str();
}

const char * LineReader::read_line(unsigned line)
{
    set_line(line);
    return next_line();
}

void LineReader::set_line(unsigned line)
{
    cur_line = line;
    f.seekg(line_pos[line]);
}

unsigned LineReader::get_line() const
{
    return cur_line;
}

void LineReader::delete_trailing_cr()
{
    auto it = buff.end() - 1;
    if (*it == '\r') {
        buff.erase(it);
    }
}
