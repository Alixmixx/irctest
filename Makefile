all:
	c++ -Wall -Wextra -Werror -g3 -std=c++98 -o server server.cpp

clean:
	rm server