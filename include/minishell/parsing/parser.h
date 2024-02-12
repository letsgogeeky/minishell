#ifndef PARSER_H
# define PARSER_H

# include "minishell/minishell.h"

typedef enum e_token_type
{
	WORD,
	OPTION,
	ASSIGNMENT_WORD,
	DLESS,
	DGREAT,
	LESS,
	GREAT,
	PIPE,
	EOF_TOKEN ,
	ERROR_TOKEN
}	t_token_type;


typedef struct s_token
{
  t_token_type	type;
  char			*lexeme;
  int			line;
} t_token;

typedef enum e_ast_node_type
{
	N_COMMAND,
	N_PIPE,
	N_CMD_WORD,
	N_CMD_PREFIX,
	N_CMD_SUFFIX,
	N_CMD_PARAM,
	N_INFILE,
	N_OUTFILE,
	N_HEREDOC,
	N_ERROR
}	t_ast_node_type;

typedef struct s_ast_node t_ast_node;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	t_ast_node 			*parent;
	t_ast_node 			*child;
	t_ast_node			*sibling;
	char				*data;
	bool				is_heredoc;
	int					fd;
}	t_ast_node;

typedef struct s_parser_state
{
	t_token		*tokens;
	int			current;
} t_parser_state;

t_ast_node	*parse_input(const char *input);
void print_ast(t_ast_node *node, int level);
t_token	*lex(const char *input);
// #include <stdbool.h>

// typedef enum ast_node_type_s
// {
// 	COMMAND,
// 	PIPE,
// 	REDIRECTION_IN,
// 	REDIRECTION_OUT,
// 	REDIRECTION_WORD
// }	ast_node_type_t;

// typedef struct ast_node_s
// {
// 	ast_node_type_t		type;
// 	struct ast_node_s	*parent;
// 	struct ast_node_s	*first_child;
// 	struct ast_node_s	*next_sibling;
// 	struct ast_node_s	*last_child;
// 	char				*value;
// 	char				*filename;				
// }	ast_node_t;








#endif