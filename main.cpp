#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

#include "LineReader.h"
#include "csv_parse.h"
#include "TableEntry.h"

#include <cstring>

static constexpr int INPUT_FILE_CNT = 2;
// arguments
enum Args {
    ARG_COUNT = 8 + 1,
    ARG_FILE0 = 1,
    ARG_FILE1,
    ARG_OUT_FILE,
    ARG_POLICY0,
    ARG_PRICE0,
    ARG_POLICY1,
    ARG_PRICE1,
    ARG_FULLNAME
};



using namespace std;

int main(int argc, char *argv[])
{
    /* Configuration */
    // Here we have to configure all the elem_positions in tables and other
    // static variables
    if (argc < ARG_COUNT) {
        return 1;
    }
    LineReader lr[INPUT_FILE_CNT] = {argv[ARG_FILE0], argv[ARG_FILE1]};
    TableEntry::elem_positions = { strtoul(argv[ARG_POLICY0], NULL, 0),
            strtoul(argv[ARG_PRICE0], NULL, 0) };
    Discrepancy::elem_positions[0] = strtoul(argv[ARG_FULLNAME], NULL, 0);
    Discrepancy::lr = lr;

    vector<TableEntry> entries[INPUT_FILE_CNT];
    // Store all entries from files
    for (int i = 0; i < INPUT_FILE_CNT; ++i) {
        const char *line;
        for (unsigned line_num = 0; (line = lr[i].next_line()); ++line_num) {
            entries[i].emplace_back(line_num, line);
        }
        // reconfigure
        if (i == 0) {
            TableEntry::elem_positions = { strtoul(argv[ARG_POLICY1], NULL, 0),
                    strtoul(argv[ARG_PRICE1], NULL, 0) };
        }
    }

    // Now sort them by policy_id
    for (int i = 0; i < INPUT_FILE_CNT; ++i) {
        sort(entries[i].begin(), entries[i].end());
    }

    vector<Discrepancy> dis_vec;
    vector<TableEntry>::const_iterator it[INPUT_FILE_CNT] = {
            entries[0].cbegin(), entries[1].cbegin()};
    while (it[0] != entries[0].cend() && it[1] != entries[1].cend()) {
        if (*it[0] == *it[1]) {
            if (it[0]->price != it[1]->price) {
                dis_vec.emplace_back(*it[0], *it[1]);
            }
            ++it[0];
            ++it[1];
        } else {
            int min_table = (*it[0] < *it[1]) ? 0 : 1;
            dis_vec.emplace_back(min_table, *it[min_table]);
            ++it[min_table];
        }
    }
    // add all the rest to the discrepancy table
    const unsigned ind = (it[0] == entries[0].cend()) ? 1 : 0;
    for (; it[ind] != entries[ind].cend(); ++it[ind]) {
        dis_vec.emplace_back(ind, *it[ind]);
    }

    // print the out
    ofstream out_f(argv[ARG_OUT_FILE]);
    for (const Discrepancy &dis : dis_vec) {
        out_f << dis;
    }
}
