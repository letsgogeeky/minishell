
NAME:= minishell

BASELIB := ./lib/ft-baselib
CFLAGS	:= -Wextra -Wall -Werror -g
LINKS := -lreadline -L/usr/local/opt/readline/lib
HEADERS := -I ./include -I ${BASELIB}/include -I/usr/local/opt/readline/include
SRC_DS := ds/stack.c
SRC_ERROR := error/execution_error.c
SRC_BUILTINS := execution/builtins.c \
	execution/builtins/pwd.c execution/builtins/env.c \
	execution/builtins/exit.c execution/builtins/echo.c \
	execution/builtins/export.c execution/builtins/unset.c \
	execution/builtins/cd.c
SRC_EXECUTION := execution/command_path.c execution/executor/executor.c \
	execution/io/io.c execution/io/sync.c execution/executor/ast_utils.c \
	execution/executor/ast_analysis_utils.c execution/executor/utils.c \
	execution/here_doc.c
SRC_EXPANDER := expander/expander.c
SRC_PARSER :=  parsing/lexer/log.c parsing/lexer/token_mgmt.c \
	parsing/lexer/token_types.c parsing/lexer/token_complex_types.c \
	parsing/parser/ast_generation.c parsing/parser/cmd_parsing.c \
	parsing/parser/parser_core.c parsing/parser/redir_parsing.c \
	parsing/lexer/lexi_lex.c parsing/parser/parsi_parse.c 
SRC_ENV := env/manager.c env/utils.c
SRC_MAIN := signals.c main.c file_manager.c str_sanitizer.c destroy.c
SRCS := $(SRC_ERROR) $(SRC_BUILTINS) $(SRC_MAIN) $(SRC_EXECUTION) $(SRC_PARSER) $(SRC_EXPANDER) $(SRC_ENV) $(SRC_DS)

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

debug: 
	@$(MAKE) CFLAGS="-Wextra -Wall -Werror -g -D LOG_DETAILS=1" re

clean:
	@rm -rf $(OBJS)

fclean: clean
	@$(MAKE) fclean --directory=${BASELIB}
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
