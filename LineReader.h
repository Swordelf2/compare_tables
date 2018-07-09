#ifndef LINEREADER_H
#define LINEREADER_H

#include <vector>
#include <string>
#include <fstream>

class LineReader
{
public:
    LineReader(const char *file_name);
    const char *next_line();
    const char *read_line(unsigned line);
    void set_line(unsigned line);
    unsigned get_line() const;

    std::ifstream f;
    std::string buff;
private:
    std::vector<std::ifstream::pos_type> line_pos;

    unsigned cur_line = 0;
    inline void delete_trailing_cr();
};

#endif
