NAME	:= ircserv

S		:= srcs/
O		:= objs/
I		:= includes/
D		:= deps/

GARBAGE	:= .vscode

SRCS	+= main.cpp
SRCS	+= utils.cpp
SRCS	+= classes/Channel.cpp
SRCS	+= classes/Client.cpp
SRCS	+= classes/Server.cpp
SRCS	+= message/parseMessage.cpp
SRCS	+= message/reply.cpp
SRCS	+= message/broadcast.cpp
SRCS	+= commands/handleCap.cpp
SRCS	+= commands/handleMode.cpp
SRCS	+= commands/handleNick.cpp
SRCS	+= commands/handlePing.cpp
SRCS	+= commands/handleQuit.cpp
SRCS	+= commands/handleUser.cpp
SRCS	+= commands/handleWhois.cpp
SRCS	+= commands/handleMotd.cpp
SRCS	+= commands/handlePrivateMessage.cpp
SRCS	+= commands/handleJoin.cpp
SRCS	+= commands/handleKick.cpp
SRCS	+= commands/handleTopic.cpp
SRCS	+= commands/handleNames.cpp
SRCS	+= commands/handlePart.cpp
SRCS	+= commands/handleList.cpp
SRCS	+= commands/handleInvite.cpp

CC		:= clang++
CFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g3 -I$I

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
	@$(RM) $D $O ${GARBAGE}

fclean: clean
	@echo "$(RED)Removing executable$(END)"
	@$(RM) $(NAME)

re: fclean
	@$(MAKE) all

-include $(DEPS)
