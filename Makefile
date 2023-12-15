# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/15 20:55:14 by ramoussa          #+#    #+#              #
#    Updated: 2023/12/15 21:07:25 by ramymoussa       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME:= minishell

CFLAGS	:= -Wextra -Wall -Werror -g
HEADERS := -I ./include
SRCS := main.c
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