#include "TableEntry.h"

#include "csv_parse.h"
#include "LineReader.h"

#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iostream>

// TEMPORARY!!
#include <iomanip>

// positions of policy_id and price
std::vector<unsigned> TableEntry::elem_positions;
// position of full_name in both files
unsigned Discrepancy::elem_positions[2];
LineReader *Discrepancy::lr = NULL;

/* ## TableEntry class ## */

TableEntry::TableEntry(unsigned line_num, const char *line) : line_num(line_num)
{
    std::vector<std::pair<const char *, size_t>> els;
    try {
        els = csv_parse_line(line, elem_positions);
    } catch (unsigned col) {
        throw_format_exc(line_num, col);
    }

    // set policy_id
    policy_id = std::string(els[0].first, els[0].second);

    // set price
    const char *price_str = els[1].first;
    char * end_ptr;
    price = strtoul(price_str, &end_ptr, 0);
    price *= 100u;
    if (*end_ptr == ',' || *end_ptr == '.') {
        ++end_ptr;
        unsigned cnt = price_str + els[1].second - end_ptr;
        if (cnt > 2 || cnt == 0) {
            throw_format_exc(line_num, elem_positions[1]);
        }
        unsigned leftover = strtoul(end_ptr, &end_ptr, 0);
        if (end_ptr != price_str + els[1].second) {
            throw_format_exc(line_num, elem_positions[1]);
        }
        if (cnt == 1) {
            leftover *= 10;
        }
        price += leftover;
    }
}

bool operator<(const TableEntry &te1, const TableEntry &te2)
{
    return te1.policy_id < te2.policy_id;
}

bool operator==(const TableEntry &te1, const TableEntry &te2)
{
    return te1.policy_id == te2.policy_id;
}

std::ostream& operator<<(std::ostream& os, const TableEntry &te)
{
    return os << std::setw(3) << te.line_num <<
            std::setw(8) << te.policy_id <<
            std::setw(8) << te.price << std::endl;
}

void TableEntry::throw_format_exc(unsigned line_num, unsigned col_num)
{
    std::ostringstream oss;
    oss << "Неверный формат ячейки" << std::endl <<
            "Строка " << line_num << ", столбец " <<
            static_cast<char>('A' + col_num);
    throw std::runtime_error(oss.str());
}

/* ## Discrepancy class ## */

// Constructor of type, present in both tables
Discrepancy::Discrepancy(const TableEntry &te0, const TableEntry &te1)
    : policy_id(te0.policy_id), prices{te0.price, te1.price}, type(-1)
{
    const char *line = lr[0].read_line(te0.line_num);
    auto v = csv_parse_line(line, { elem_positions[0] });
    full_name = std::string(v[0].first, v[0].second);
}

// Constructor of type, present only in one table
Discrepancy::Discrepancy(unsigned table_ind, const TableEntry &te)
    : policy_id(te.policy_id), type(table_ind)
{
    prices[table_ind] = te.price;
    prices[!table_ind] = 0.0;
    const char *line = lr[table_ind].read_line(te.line_num);
    auto v = csv_parse_line(line, { elem_positions[table_ind] });
    full_name = std::string(v[0].first, v[0].second);
}

std::ostream& operator<<(std::ostream& os, const Discrepancy &dis)
{
    os << dis.full_name << ';' << dis.policy_id << ';';
    if (dis.type == -1) {
        os << Discrepancy::uint_to_money(dis.prices[0]) << ';' <<
                Discrepancy::uint_to_money(dis.prices[1]);
    } else {
        if (dis.type == 0) {
            os << Discrepancy::uint_to_money(dis.prices[0]) << ';';
        } else {
            os << ';' << Discrepancy::uint_to_money(dis.prices[1]);
        }
    }
    os << ';';
    unsigned diff;
    if (dis.prices[0] > dis.prices[1]) {
        os << '+';
        diff = dis.prices[0] - dis.prices[1];
    } else {
        os << '-';
        diff = dis.prices[1] - dis.prices[0];
    }
    os << Discrepancy::uint_to_money(diff);

    return os << std::endl;
}

std::string Discrepancy::uint_to_money(unsigned price)
{
    std::ostringstream oss;
    unsigned q = price / 100u;
    unsigned rem = price % 100u;
    oss << q << ',';
    if (rem < 10u) {
        oss << '0';
    }
    oss << rem;
    return oss.str();
}
