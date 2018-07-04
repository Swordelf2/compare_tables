#include <iostream>
#include <vector>
#include <utility>

#include "csv.h"
#include "csv_parse.h"

int main()
{
    io::LineReader lr("test");

    auto v = csv_parse_line(lr.next_line(), {1, 4});
    std::cout << v.size() << ':' << std::string(v[0].first, v[0].second)
        << ';' << std::string(v[1].first, v[1].second) << std::endl;
    v = csv_parse_line(lr.next_line(), {1, 4});
    std::cout << v.size() << ':' << std::string(v[0].first, v[0].second)
        << ';' << std::string(v[1].first, v[1].second) << std::endl;
}
