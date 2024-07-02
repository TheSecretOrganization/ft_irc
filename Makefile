### PROGRAM NAME ###
NAME        := ircserv

### UTILS ###
CC          := c++
CPPFLAGS    := -Wall -Werror -Wextra -std=c++98
DEP_FLAGS   := -MMD -MP
MAKE_FLAG   := --no-print-directory --silent
RM          := rm -rf
ifeq (debug, $(filter debug,$(MAKECMDGOALS)))
    CPPFLAGS += -g3
endif

### DIRECTORIES ###
SRC_DIR     := src
CMDS_DIR	:= commands
INCLD_DIR   := include
OBJS_DIR    := objs

### FILES ###
SRC         :=	main.cpp \
	Channel.cpp \
	Client.cpp \
	ClientSocket.cpp \
	Command.cpp \
	CommandRegistry.cpp \
	Configuration.cpp \
	Server.cpp \
	ServerSocket.cpp \
	Socket.cpp \
	SocketObserver.cpp \
	$(addprefix $(CMDS_DIR)/, \
		CapCommand.cpp \
		InviteCommand.cpp \
		JoinCommand.cpp \
		KickCommand.cpp \
		LusersCommand.cpp \
		ModeCommand.cpp \
		MotdCommand.cpp \
		NamesCommand.cpp \
		NickCommand.cpp \
		PartCommand.cpp \
		PassCommand.cpp \
		PingCommand.cpp \
		PrivmsgCommand.cpp \
		QuitCommand.cpp \
		TopicCommand.cpp \
		UserCommand.cpp \
		WhoCommand.cpp \
	)
INCLUDES    :=  $(INCLD_DIR)
INCLD_FLAG  :=  $(addprefix -I , $(INCLUDES))
OBJS        :=  $(patsubst %.cpp,$(OBJS_DIR)/%.o,$(SRC))
DEPS        :=  $(patsubst %.cpp,$(OBJS_DIR)/%.d,$(SRC))

### COLORS ###
DEFAULT     := \033[0m
BLACK       := \033[0;30m
RED         := \033[0;31m
GREEN       := \033[0;32m
UGREEN      := \033[4;32m
YELLOW      := \033[;33m
BLUE        := \033[0;34m
PURPLE      := \033[0;35m
CYAN        := \033[0;36m
BWHITE      := \033[1;37m
NEW         := \r\033[K

### PROJECT ###
all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(NEW)$(PURPLE)[$(NAME)] $(UGREEN)Building:$(DEFAULT)$(BWHITE) $@$(DEFAULT)"
	@$(CC) $(CPPFLAGS) $(OBJS) $(INCLD_FLAG) -o $@
	@printf "\n"

-include $(DEPS)
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	@printf "$(NEW)$(PURPLE)[$(NAME)] $(UGREEN)Building:$(DEFAULT) $<"
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/$(CMDS_DIR)
	@$(CC) $(DEP_FLAGS) $(CPPFLAGS) $(INCLD_FLAG) -c $< -o $@

clean:
	@printf "$(PURPLE)[$(NAME)] $(RED)Removing $(DEFAULT)$(OBJS_DIR) files\n"
	@$(RM) $(OBJS_DIR)

fclean: clean
	@printf "$(PURPLE)[$(NAME)] $(RED)Removing $(DEFAULT)$(NAME)\n"
	@$(RM) $(NAME)

re: fclean all

cre:
	@clear
	@make re $(MAKE_FLAG)

debug: fclean all

.PHONY: all clean fclean re debug
