#include "TableEntry.h"

#include <string>
#include <array>
#include <cstdlib>

std::array<unsigned, ENTRY_ELEM_CNT> TableEntry::elem_positions = {2, 6};

TableEntry::TableEntry(char *line)
{
    unsigned cur_pos = 0;
    // get policy_id
    while (cur_pos < elem_positions[0]) {
        // skip to next element in the row
        while (*line && *line != ';') {
            ++line;
        }
        ++cur_pos;
        if (*line == '\0') {
            throw std::logic_error("Invalid file format");
        }
        ++line;
    }
    char *end_elem = line;
    while (*end_elem && *end_elem != ';') {
        ++end_elem;
    }

    policy_id = std::string(line, end_elem - line);
    line = end_elem;
    if (*line == '\0') {
        throw std::logic_error("Invalid file format");
    }

    // get price
    while (cur_pos < elem_positions[1]) {
        // skip to next element in the row
        while (*line && *line != SEPERATOR) {
            ++line;
        }
        ++cur_pos;
        if (*line == '\0') {
            throw std::logic_error("Invalid file format");
        }
    }
    end_elem = line;
    while (*end_elem && *end_elem != SEPERATOR) {
        // change ',' to decimal dot
        if (*end_elem == ',') {
            *end_elem = '.';
            break;
        }
        ++end_elem;
    }
    price = strtod(line, NULL);
}

void TableEntry::set_elem_positions(const std::array<unsigned, ENTRY_ELEM_CNT> &el_pos_arr)
{
    elem_positions = el_pos_arr;
}
