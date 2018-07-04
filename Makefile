SRC=TableEntry.cpp main.cpp csv_parse.cpp
ct: $(SRC)
	g++ -std=gnu++17 -Wall -Wextra -g $^ -o $@ -lpthread
