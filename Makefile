NAME := ircserv
PATH_OBJS := objs
CXX := c++ -Wall -Wextra -Werror -g3 -std=c++98
SRCS := server.cpp
OBJS := ${patsubst %.cpp, ${PATH_OBJS}/%.o, ${SRCS}}

all: ${NAME}

${OBJS}: ${PATH_OBJS}/%.o: %.cpp
	@mkdir -p ${PATH_OBJS}
	${CXX} -c $< -o $@

${NAME}: ${OBJS}
	${CXX} ${OBJS} -o $@

clean:
	rm -rf ${PATH_OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean ${NAME}

.PHONY: all clean fclean re
