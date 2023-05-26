NAME		:= ircserv

S			:= srcs/
O			:= objs/
I			:= includes/
D			:= deps/

GARBAGE		:= .vscode classes commands message

CXX			:= c++
CXXLIBS		:= -lcurl # -lcurl for bonus
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -fsanitize=thread -fno-omit-frame-pointer -g -I$I
VALGRIND	:= valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -q

SRCS		+= srcs/clean.cpp
SRCS		+= srcs/main.cpp
SRCS		+= srcs/utils.cpp
SRCS		+= srcs/classes/Channel.cpp
SRCS		+= srcs/classes/Client.cpp
SRCS		+= srcs/classes/FormerClient.cpp
SRCS		+= srcs/classes/Server.cpp
SRCS		+= srcs/classes/Bot.cpp
SRCS		+= srcs/message/reply.cpp
SRCS		+= srcs/message/parseMessage.cpp
SRCS		+= srcs/commands/handleAdmin.cpp
SRCS		+= srcs/commands/handleInvite.cpp
SRCS		+= srcs/commands/handleJoin.cpp
SRCS		+= srcs/commands/handleKick.cpp
SRCS		+= srcs/commands/handleList.cpp
SRCS		+= srcs/commands/handleLusers.cpp
SRCS		+= srcs/commands/handleMode.cpp
SRCS		+= srcs/commands/handleMotd.cpp
SRCS		+= srcs/commands/handleNames.cpp
SRCS		+= srcs/commands/handleNick.cpp
SRCS		+= srcs/commands/handleOper.cpp
SRCS		+= srcs/commands/handlePart.cpp
SRCS		+= srcs/commands/handlePass.cpp
SRCS		+= srcs/commands/handlePing.cpp
SRCS		+= srcs/commands/handlePrivateMessage.cpp
SRCS		+= srcs/commands/handleQuit.cpp
SRCS		+= srcs/commands/handleTime.cpp
SRCS		+= srcs/commands/handleTopic.cpp
SRCS		+= srcs/commands/handleUser.cpp
SRCS		+= srcs/commands/handleUserhost.cpp
SRCS		+= srcs/commands/handleVersion.cpp
SRCS		+= srcs/commands/handleWho.cpp
SRCS		+= srcs/commands/handleWhois.cpp
SRCS		+= srcs/commands/handleWhowas.cpp

FILENAMES	:= $(basename $(SRCS))
FOLDERS 	:= $(sort $(dir $(SRCS)))
OBJS		:= $(FILENAMES:$S%=$O%.o)
DEPS		:= $(FILENAMES:$S%=$D%.d)

RM			:= rm -rf
MKDIR		:= mkdir -p

END			:= \033[0m
RED			:= \033[31m
GREEN		:= \033[32m
BLUE		:= \033[34m

all: $(NAME)

$O:
	$(MKDIR) $(FOLDERS:$(S)%=$(O)%)

$(OBJS): $O%.o: $S%.cpp | $O
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "$(GREEN)✓ $@$(END)"

$(DEPS): $D%.d: $S%.cpp
	@$(MKDIR) $(FOLDERS:$(S)%=$(D)%)
	@$(CXX) $(CXXFLAGS) -MM -MF $@ -MT "$O$*.o $@" $<

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(CXXLIBS)
	@echo "$(BLUE)$(NAME) is compiled.$(END)"

clean:
	@echo "$(RED)Removing $D and $O$(END)"
	@$(RM) $D $O ${GARBAGE}

fclean: clean
	@echo "$(RED)Removing executable$(END)"
	@$(RM) $(NAME)

re: fclean
	@$(MAKE) all

bonus: $(NAME)

.PHONY: all bonus clean fclean re

-include $(DEPS)