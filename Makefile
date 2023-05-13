all:
	c++ -Wall -Wextra -Werror -std=c++98 -o server server.cpp

clean:
	rm server