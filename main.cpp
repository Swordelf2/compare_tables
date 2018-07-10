#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <cctype>

#include "LineReader.h"
#include "csv_parse.h"
#include "TableEntry.h"

#include <cstring>

static constexpr int INPUT_FILE_CNT = 2;
static constexpr const char * CONFIG_FILE_NAME = "config.txt";

// arguments
enum Args {
    ARG_COUNT = 3 + 1,
    ARG_FILE0 = 1,
    ARG_FILE1,
    ARG_OUT_FILE,
};

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < ARG_COUNT) {
        return 1;
    }

    /* Configuration */
    // Here we have to configure all the elem_positions in tables and other
    // static variables
    LineReader lr[INPUT_FILE_CNT] = {argv[ARG_FILE0], argv[ARG_FILE1]};
    Config cfg(CONFIG_FILE_NAME);
    Discrepancy::lr = lr;

    vector<TableEntry> entries[INPUT_FILE_CNT];
    // Store all entries from files
    for (int i = 0; i < INPUT_FILE_CNT; ++i) {
        TableEntry::Configure(lr, i, cfg);
        const char *line;
        while ((line = lr[i].next_line())
                && isdigit(static_cast<unsigned char>(line[0]))) {
            entries[i].emplace_back(lr[i].get_line() - 1, line);
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
