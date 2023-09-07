CC			=	clang++
CC_FLAGS	=	-Wall -Wextra -Werror -fno-builtin

DIR_HEADERS	=	./includes/
DIR_SRCS	=	./srcs/
DIR_OBJS	=	./compiled_srcs/

SRCS		=	matt_daemon.cpp \
				Tintin_reporter.cpp \
				Lock.cpp \
				Server.cpp

INCLUDES	=	matt_daemon.hpp \
				Tintin_reporter.hpp \
				Lock.hpp \
				Server.hpp

OBJS		=	$(SRCS:%.cpp=$(DIR_OBJS)%.o)
DEPS		=	$(SRCS:%.cpp=$(DIR_OBJS)%.d)

NAME		=	matt_daemon

ifeq ($(BUILD),debug)
	CC_FLAGS		+=	-DDEBUG -g3 -fsanitize=address
	DIR_OBJS		=	./debug-compiled_srcs/
	NAME			=	debug-matt_daemon
endif

all:		$(NAME)

$(NAME):	$(OBJS) $(addprefix $(DIR_HEADERS), $(INCLUDES))
			$(CC) $(CC_FLAGS) -I $(DIR_HEADERS) $(OBJS) -o $(NAME)

$(OBJS):	| $(DIR_OBJS)

$(DIR_OBJS)%.o:		$(DIR_SRCS)%.cpp Makefile
					@mkdir -p $(dir $@)
					$(CC) $(CC_FLAGS) -MMD -MP -I $(DIR_HEADERS) -c $< -o $@
-include			$(DEPS)

$(DIR_OBJS):
				mkdir -p $(DIR_OBJS)

clean:
				rm -rf $(DIR_OBJS)

fclean:			clean
				rm -rf $(NAME)

re:				fclean
				$(MAKE) --no-print-directory

.PHONY:			all clean fclean re
