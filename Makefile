# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/15 20:55:14 by ramoussa          #+#    #+#              #
#    Updated: 2023/12/16 19:05:18 by ramymoussa       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= minishell

CFLAGS	:= -g -lreadline # -Wextra -Wall -Werror 
HEADERS := -I ./include

SRC_ERROR := error/execution_error.c error/parser_error.c
SRC_BUILTINS := execution/builtins/pwd.c
SRC_MAIN := main.c 

SRCS := $(SRC_ERROR) $(SRC_BUILTINS) $(SRC_MAIN) 

OBJS := ${addprefix src/, ${SRCS:.c=.o}}

all: ${NAME}

%.o: %.c
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) $(OBJS) $(HEADERS) -o $(NAME) && echo "Successful $(NAME) build...!"

clean:
	@rm -rf $(OBJS)

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re