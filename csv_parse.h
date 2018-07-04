#ifndef CSV_PARSE_H
#define CSV_PARSE_H

#include <vector>

std::vector<std::pair<char *, size_t>>
csv_parse_line(char *line, const std::vector<unsigned> &elem_positions);

#endif
