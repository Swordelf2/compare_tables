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
    const char *read_line(unsigned line_num);

    std::ifstream f;
    std::string buff;
private:
    std::vector<std::ifstream::pos_type> line_pos;
};

#endif
