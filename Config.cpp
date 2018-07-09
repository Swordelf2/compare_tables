#include "Config.h"

#include <fstream>
#include <string>

Config::Config(const char *file_name)
{
    std::ifstream f(file_name);
    if (!f) {
        throw std::runtime_error("Could not open config file");
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < FIELDS_CNT; ++j) {
            read_next_line(f, names[i][j]);
        }
    }
}

std::ifstream& Config::read_next_line(std::ifstream &f, std::string &s)
{
    // skip all comments
    do {
        getline(f, s);
    } while (s[0] == '#');
    return f;
}
