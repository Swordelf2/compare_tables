#ifndef TABLEENTRY_H
#define TABLEENTRY_H

#include <string>
#include <array>

enum
{
    ENTRY_ELEM_CNT = 2,
};

class TableEntry
{
public:
    TableEntry(char *line);

    static void set_elem_positions(const std::array<unsigned, ENTRY_ELEM_CNT> &el_pos_arr);
    
    std::string policy_id;
    double price;
private:
    static std::array<unsigned, ENTRY_ELEM_CNT> elem_positions;
    static constexpr char SEPERATOR = ';';
};

#endif
