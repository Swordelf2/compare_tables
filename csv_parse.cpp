#include "csv_parse.h"

#include <vector>
#include <utility>
#include <stdexcept>

constexpr char QUOTE = '"';
constexpr char SEP = ';';

std::vector<std::pair<const char *, size_t>>
csv_parse_line(const char *line, const std::vector<unsigned> &elem_positions)
{
    std::vector<std::pair<const char *, size_t>> result;
    result.reserve(elem_positions.size());
    unsigned cur_pos = 0;
    for (const unsigned &el_pos : elem_positions) {
        while (cur_pos < el_pos) {
            char last = SEP;
            if (*line == QUOTE) {
                last = QUOTE;
                ++line;
            } else if (*line == '\0') {
                throw cur_pos;
            }
            while (*line && *line != last) {
                ++line;
            }
            if (last == QUOTE) {
                if (*line == QUOTE && (*(line + 1) == SEP)) {
                    line += 2;
                } else if (*line == QUOTE && (*(line + 1) == '\0')) {
                    line += 1;
                } else {
                    throw cur_pos;
                }
            } else if (*line) {
                ++line;
            }
            ++cur_pos;
        }

        const char *end;
        char last = SEP;
        if (*line == QUOTE) {
            last = QUOTE;
            ++line;
        } else if (*line == '\0') {
            throw cur_pos;
        }
        end = line;

        while (*end && *end != last) {
            ++end;
        }

        const char *start = line;
        if (last == QUOTE) {
            if (*end == QUOTE && (*(end + 1) == SEP)) {
                line = end + 2;
            } else if (*end == QUOTE && (*(end + 1) == '\0')) {
                line = end + 1;
            } else {
                throw cur_pos;
            }
        } else if (*end) {
            line = end + 1;
        }
        result.emplace_back(start, end - start);
        ++cur_pos;
    }

    return result;
}
