# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/15 20:55:14 by ramoussa          #+#    #+#              #
#    Updated: 2024/01/07 01:18:56 by ramoussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= minishell

BASELIB := ./lib/ft-baselib
CFLAGS	:= -Wextra -Wall -Werror -g
LINKS := -lreadline
HEADERS := -I ./include -I ${BASELIB}/include
SRC_ERROR := error/execution_error.c error/parser_error.c
SRC_BUILTINS := execution/builtins/pwd.c
SRC_EXECUTION := execution/command_path.c execution/executor.c \
	execution/io.c
SRC_MAIN := signals.c main.c

SRCS := $(SRC_ERROR) $(SRC_BUILTINS) $(SRC_MAIN) $(SRC_EXECUTION)

OBJS := ${addprefix src/, ${SRCS:.c=.o}}
LIBS := ${BASELIB}/baselib.a

all: BASELIB ${NAME}

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

${NAME}: $(LIBS) $(OBJS)
	@$(CC) $(OBJS) $(LINKS) $(LIBS) $(HEADERS) -o $(NAME) && echo "Successful $(NAME) build...!"

BASELIB:
	@if [ -d ${BASELIB} ]; then\
		echo "${BASELIB} already exists... proceeding to next step.";\
	else\
		mkdir lib;\
		git clone git@github.com:letsgogeeky/ft-baselib.git ${BASELIB};\
	fi
	make --directory=${BASELIB}

clean:
	@rm -rf $(OBJS)

fclean: clean
	@$(MAKE) fclean --directory=${BASELIB}
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
