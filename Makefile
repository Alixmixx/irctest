NAME		:= ircserv

S			:= srcs/
O			:= objs/
I			:= includes/
D			:= deps/

GARBAGE		:= .vscode

CXX			:= clang++ # TODO c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g3 -I$I
VALGRIND	:= valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -q

FILENAMES	+= main
FILENAMES	+= utils
FILENAMES	+= classes/Channel
FILENAMES	+= classes/Client
FILENAMES	+= classes/Server
FILENAMES	+= commands/ignoreCommand
FILENAMES	+= commands/handleInvite
FILENAMES	+= commands/handleJoin
FILENAMES	+= commands/handleKick
FILENAMES	+= commands/handleList
FILENAMES	+= commands/handleMode
FILENAMES	+= commands/handleMotd
FILENAMES	+= commands/handleNames
FILENAMES	+= commands/handleNick
FILENAMES	+= commands/handleOper
FILENAMES	+= commands/handlePart
FILENAMES	+= commands/handlePing
FILENAMES	+= commands/handlePrivateMessage
FILENAMES	+= commands/handleQuit
FILENAMES	+= commands/handleTopic
FILENAMES	+= commands/handleUser
FILENAMES	+= commands/handleWhois
FILENAMES	+= message/broadcast
FILENAMES	+= message/parseMessage
FILENAMES	+= message/reply

FOLDERS 	:= $(sort $(dir $(FILENAMES)))
SRCS		:= $(addprefix $S, $(addsuffix .cpp, $(FILENAMES)))
OBJS		:= $(FILENAMES:%=$O%.o)
DEPS		:= $(FILENAMES:%=$D%.d)

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
	@$(CXX) $^ -o $@
	@echo "$(BLUE)$(NAME) is compiled.$(END)"

clean:
	@echo "$(RED)Removing $D and $O$(END)"
	@$(RM) $D $O ${GARBAGE}

fclean: clean
	@echo "$(RED)Removing executable$(END)"
	@$(RM) $(NAME)

re: fclean
	@$(MAKE) all

run: $(NAME)
	@$(VALGRIND) ./$(NAME) 6667 password

bonus: $(NAME)

.PHONY: all bonus clean fclean re run

-include $(DEPS)