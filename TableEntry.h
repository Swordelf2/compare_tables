#ifndef TABLEENTRY_H
#define TABLEENTRY_H

#include <string>
#include <vector>
#include <iostream>

#include "LineReader.h"

class TableEntry
{
public:
    TableEntry(unsigned line_num, const char *line);

    friend bool operator<(const TableEntry &te1, const TableEntry &te2);
    friend bool operator==(const TableEntry &te1, const TableEntry &te2);
    friend std::ostream& operator<<(std::ostream& os, const TableEntry &te);

    unsigned line_num;
    std::string policy_id;
    unsigned price;

    static std::vector<unsigned> elem_positions;
private:
    static constexpr int ENTRY_ELEM_CNT = 3;
    static inline void throw_format_exc(unsigned line_num, unsigned col_num);
};

class Discrepancy
{
public:
    Discrepancy();
    Discrepancy(const TableEntry &te0, const TableEntry &te1);
    Discrepancy(unsigned table_ind, const TableEntry &te);

    friend std::ostream& operator<<(std::ostream& os, const Discrepancy &dis);

    std::string full_name;
    std::string policy_id;
    unsigned prices[2];

    static unsigned elem_positions[2];
    static LineReader *lr;
private:
    // -1 for both, otherwise the number of the table in whcih the entry is present
    int type;

    static std::string uint_to_money(unsigned price);
};

#endif
