#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>

class Config
{
public:
    static constexpr int FIELDS_CNT = 5;
    Config(const char *file_name);

    std::string names[2][FIELDS_CNT];
        /*
        full_name;
        policy;
        date0;
        date1;
        price;
        */
private:
    std::ifstream& read_next_line(std::ifstream &f, std::string &s);
};

#endif
