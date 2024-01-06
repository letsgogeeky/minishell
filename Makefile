# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/15 20:55:14 by ramoussa          #+#    #+#              #
#    Updated: 2024/01/06 19:16:48 by ramymoussa       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= minishell

BASELIB := ./lib/ft-baselib
CFLAGS	:= -lreadline -Wextra -Wall -Werror -g
HEADERS := -I ./include -I ${BASELIB}/include
SRC_ERROR := error/execution_error.c error/parser_error.c
SRC_BUILTINS := execution/builtins/pwd.c
SRC_EXECUTION := execution/command_path.c execution/executor.c \
	execution/io.c
SRC_MAIN := main.c

SRCS := $(SRC_ERROR) $(SRC_BUILTINS) $(SRC_MAIN) $(SRC_EXECUTION)

OBJS := ${addprefix src/, ${SRCS:.c=.o}}
LIBS := ${BASELIB}/baselib.a

all: BASELIB ${NAME}

%.o: %.c
	@$(CC) $(HEADERS) -o $@ -c $<

${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME) && echo "Successful $(NAME) build...!"

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
