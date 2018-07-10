#ifndef TABLEENTRY_H
#define TABLEENTRY_H

#include <string>
#include <vector>
#include <iostream>

#include "Config.h"
#include "LineReader.h"

enum
{
    EL_FULL_NAME = 0,
    EL_POLICY,
    EL_DATE0,
    EL_DATE1,
    EL_PRICE
};

class TableEntry
{
public:
    TableEntry(unsigned line_num, const char *line);

    friend bool operator<(const TableEntry &te1, const TableEntry &te2);
    friend bool operator==(const TableEntry &te1, const TableEntry &te2);
    friend std::ostream& operator<<(std::ostream& os, const TableEntry &te);

    static void Configure(LineReader lrs[], unsigned table_ind, const Config &cfg);

    unsigned line_num;
    std::string policy_id;
    std::string date[2];
    unsigned price;

    static unsigned elem_positions[2][Config::FIELDS_CNT];
private:
    static unsigned cur_table;
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
    std::string date[2];
    unsigned prices[2];

    static LineReader *lr;
    static std::string uint_to_money(unsigned price);
private:
    // -1 for both, otherwise the number of the table in whcih the entry is present
    int type;

    void read_name(const char *name, size_t cnt);
};

#endif
