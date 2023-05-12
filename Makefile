all:
	c++ -Wall -Wextra -Werror -std=c++11 -o server -pthread server.cpp

clean:
	rm server