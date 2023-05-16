S		= srcs/

O		= objs/

I		= includes/

D		= deps/

NAME	= ircserv

SRCS	= 	main.cpp\
			Server.cpp\
			Client.cpp\
			Channel.cpp\
			Init/InitCommandHandlerMap.cpp\
			Init/InitReplyMap.cpp\
			Init/InitServerDateAndTime.cpp\
			Message/ParseMessage.cpp\
			Message/Reply.cpp\
			Message/Broadcast.cpp\
			Commands/ConnectionCommands/HandleCap.cpp\
			Commands/ConnectionCommands/HandleMode.cpp\
			Commands/ConnectionCommands/HandleNick.cpp\
			Commands/ConnectionCommands/HandlePing.cpp\
			Commands/ConnectionCommands/HandleQuit.cpp\
			Commands/ConnectionCommands/HandleUser.cpp\
			Commands/ConnectionCommands/HandleWhois.cpp\
			Commands/ConnectionCommands/HandleMotd.cpp\
			Commands/SendingCommands/HandlePrivateMessage.cpp\
			Commands/ChannelCommands/HandleJoin.cpp\

FOLDERS = $(sort $(dir $(SRCS)))

CC		= c++

CFLAGS	+= -Wall -Wextra -Werror -std=c++98

LDFLAGS	+=

CFLAGS	+= -I$I

SRCS	:= $(foreach file,$(SRCS),$S$(file))
OBJS	= $(SRCS:$S%=$O%.o)
DEPS	= $(SRCS:$S%=$D%.d)

RM		= /bin/rm -rf

END=\033[0m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

.PHONY: all clean fclean re

all: $(NAME)

$O:
	mkdir -p $@ $(FOLDERS:$(S)%=$(O)%)

$(OBJS): | $O

$(OBJS): $O%.o: $S%
	@echo "$(YELLOW)Compiling $^: $(END)"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)✓$(END)"

$D:
	mkdir -p $@ $(FOLDERS:$(S)%=$(D)%)

$(DEPS): | $D

$(DEPS): $D%.d: $S%
	@$(CC) $(CFLAGS) -MM -MF $@ -MT "$O$*.o $@" $<

$(NAME): $(OBJS)
	@echo "$(MAGENTA)$(NAME) is compiled$(END)"
	@$(CC) $(LDFLAGS) $^ -o $@

clean:
	@echo "$(RED)Removing objs$(END)"
	@$(RM) $D $O

fclean: clean
	@echo "$(RED)Removed executable$(END)"
	@$(RM) $(NAME)

re: fclean all

-include $(DEPS)
