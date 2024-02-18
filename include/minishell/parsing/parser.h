/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 21:08:54 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/18 21:10:51 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell/parsing/lexer.h"

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

t_ast_node	*parse_input(const char *input);
void		print_ast(t_ast_node *node, int level);
void		destroy_ast(t_ast_node *node);

int			open_file(char *path, int flags);

#endif