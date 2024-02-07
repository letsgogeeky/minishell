
NAME:= minishell

BASELIB := ./lib/ft-baselib
CFLAGS	:= -Wextra -Wall -Werror -g
LINKS := -lreadline -L/usr/local/opt/readline/lib
HEADERS := -I ./include -I ${BASELIB}/include -I/usr/local/opt/readline/include
SRC_ERROR := error/execution_error.c error/parser_error.c
SRC_BUILTINS := execution/builtins.c \
	execution/builtins/pwd.c execution/builtins/env.c \
	execution/builtins/exit.c execution/builtins/echo.c \
	execution/builtins/export.c execution/builtins/unset.c \
	execution/builtins/cd.c
SRC_EXECUTION := execution/command_path.c execution/executor.c \
	execution/io.c execution/utils.c
SRC_EXPANDER := expander/expander.c
SRC_PARSER := $(wildcard parser/lexer/*.c)
SRC_ENV := env/manager.c env/utils.c
SRC_MAIN := signals.c main.c file_manager.c str_sanitizer.c
SRCS := $(SRC_ERROR) $(SRC_BUILTINS) $(SRC_MAIN) $(SRC_EXECUTION) $(SRC_PARSER) $(SRC_EXPANDER) $(SRC_ENV)

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
