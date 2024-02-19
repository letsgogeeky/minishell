/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 21:08:54 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 02:56:57 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell/parsing/lexer.h"
# include <stdbool.h>
# include <fcntl.h>
# ifndef LOG_DETAILS
#  define LOG_DETAILS 0
# endif

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

typedef struct s_ast_node	t_ast_node;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	t_ast_node			*parent;
	t_ast_node			*child;
	t_ast_node			*sibling;
	char				*data;
	bool				is_heredoc;
	int					fd;
}	t_ast_node;

typedef struct s_parser_state
{
	t_token		*tokens;
	int			current;
}	t_parser_state;

t_token			peek(t_parser_state *state);
t_token			consume(t_parser_state *state);
bool			match(t_parser_state *state, t_token_type type);

void			init_parser_state(t_parser_state *state, t_token *tokens);
t_ast_node		*create_node(t_ast_node_type type);
char			*node_type_to_string(t_ast_node_type type);
t_ast_node_type	determine_node_type(t_token_type type);
void			destroy_ast(t_ast_node *node);

t_ast_node		*parse_cmd_suffix(t_parser_state *state);
t_ast_node		*parse_cmd_prefix(t_parser_state *state);
t_ast_node		*parse_command(t_parser_state *state);

bool			is_redirect(t_parser_state *state);
t_ast_node		*parse_redirect(t_parser_state *state);

t_ast_node		*parse_input(const char *input);
int				open_file(char *path, int flags);

#endif