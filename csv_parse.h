#ifndef CSV_PARSE_H
#define CSV_PARSE_H

#include <vector>

constexpr char QUOTE = '"';
constexpr char SEP = ';';

std::vector<std::pair<const char *, size_t>>
csv_parse_line(const char *line, const std::vector<unsigned> &elem_positions);
std::vector<std::pair<const char *, size_t>>
csv_parse_whole_line(const char *line);

#endif
