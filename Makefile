NAME	:= ircserv

S		:= srcs/
O		:= objs/
I		:= includes/
D		:= deps/

SRCS	+= main.cpp
SRCS	+= Server.cpp
SRCS	+= Client.cpp
SRCS	+= Channel.cpp
SRCS	+= Init/InitCommandHandlerMap.cpp
SRCS	+= Init/InitReplyMap.cpp
SRCS	+= Init/InitServerDateAndTime.cpp
SRCS	+= Message/ParseMessage.cpp
SRCS	+= Message/Reply.cpp
SRCS	+= Message/Broadcast.cpp
SRCS	+= Commands/ConnectionCommands/HandleCap.cpp
SRCS	+= Commands/ConnectionCommands/HandleMode.cpp
SRCS	+= Commands/ConnectionCommands/HandleNick.cpp
SRCS	+= Commands/ConnectionCommands/HandlePing.cpp
SRCS	+= Commands/ConnectionCommands/HandleQuit.cpp
SRCS	+= Commands/ConnectionCommands/HandleUser.cpp
SRCS	+= Commands/ConnectionCommands/HandleWhois.cpp
SRCS	+= Commands/ConnectionCommands/HandleMotd.cpp
SRCS	+= Commands/SendingCommands/HandlePrivateMessage.cpp
SRCS	+= Commands/ChannelCommands/HandleJoin.cpp
SRCS	+= Commands/ChannelCommands/HandleKick.cpp
SRCS	+= Commands/ChannelCommands/HandleTopic.cpp
SRCS	+= Commands/ChannelCommands/HandleNames.cpp
SRCS	+= Utils/ParseArgv.cpp
SRCS	+= Utils/Utils.cpp

CC		:= clang++
CFLAGS	:= -g -Wall -Wextra -Werror -std=c++98 -I$I

SRCS	:= $(foreach file,$(SRCS),$S$(file))
FOLDERS := $(sort $(dir $(SRCS)))
OBJS	:= $(SRCS:$S%=$O%.o)
DEPS	:= $(SRCS:$S%=$D%.d)

RM		:= rm -rf
MKDIR	:= mkdir -p

END		:= \033[0m
RED		:= \033[0;91m
GREEN	:= \033[0;92m
MAGENTA	:= \033[0;95m

.PHONY: all clean fclean re

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
	@echo "$(MAGENTA)$(NAME) is compiled$(END)"

clean:
	@echo "$(RED)Removing $D and $O$(END)"
	@$(RM) $D $O

fclean: clean
	@echo "$(RED)Removing executable$(END)"
	@$(RM) $(NAME)

re: fclean
	@$(MAKE) all

-include $(DEPS)