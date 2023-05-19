NAME	:= ircserv

S		:= srcs/
O		:= objs/
I		:= includes/
D		:= deps/

GARBAGE	:= .vscode

CC		:= clang++ # TODO c++
CFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g3 -I$I

SRCS	:= $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
FOLDERS := $(sort $(dir $(SRCS)))
OBJS	:= $(SRCS:$S%=$O%.o)
DEPS	:= $(SRCS:$S%=$D%.d)

RM		:= rm -rf
MKDIR	:= mkdir -p

END		:= \033[0m
RED		:= \033[31m
GREEN	:= \033[32m
BLUE	:= \033[34m

all: $(NAME)

$O:
	$(MKDIR) $(FOLDERS:$(S)%=$(O)%)

$(OBJS): $O%.o: $S% | $O
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)✓ $@$(END)"

$(DEPS): $D%.d: $S%
	@$(MKDIR) $(FOLDERS:$(S)%=$(D)%)
	@$(CC) $(CFLAGS) -MM -MF $@ -MT "$O$*.o $@" $<

$(NAME): $(OBJS)
	@$(CC) $^ -o $@
	@echo "$(BLUE)$(NAME) is compiled$(END)"

clean:
	@echo "$(RED)Removing $D and $O$(END)"
	@$(RM) $D $O ${GARBAGE}

fclean: clean
	@echo "$(RED)Removing executable$(END)"
	@$(RM) $(NAME)

re: fclean
	@$(MAKE) all

.PHONY: all clean fclean re

-include $(DEPS)
