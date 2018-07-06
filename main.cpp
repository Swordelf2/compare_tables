#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

#include "LineReader.h"
#include "csv_parse.h"
#include "TableEntry.h"

static constexpr int INPUT_FILE_CNT = 2;

using namespace std;

int main()
{
    LineReader lr[INPUT_FILE_CNT] = {"test1.csv", "test2.csv"};

    /* Configuration */
    // Here we have to configure all the elem_positions in tables and other
    // static variables
    Discrepancy::lr = lr;

    vector<TableEntry> entries[INPUT_FILE_CNT];
    // Store all entries from files
    for (int i = 0; i < INPUT_FILE_CNT; ++i) {
        const char *line;
        for (unsigned line_num = 0; (line = lr[i].next_line()); ++line_num) {
            entries[i].emplace_back(line_num, line);
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
    for (const Discrepancy &dis : dis_vec) {
        cout << dis;
    }
}
