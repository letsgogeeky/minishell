/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsi_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 02:37:09 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 03:22:43 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/parser.h"
#include "baselib.h"

t_ast_node	*parse_complete_command(t_parser_state *state);

t_ast_node	*handle_pipe_sequence(t_parser_state *state, t_ast_node *cmd)
{
	t_ast_node	*pipe;
	t_ast_node	*next_cmd;

	pipe = create_node(N_PIPE);
	next_cmd = parse_complete_command(state);
	pipe->child = cmd;
	cmd->parent = pipe;
	if (next_cmd)
	{
		next_cmd->parent = pipe;
		cmd->sibling = next_cmd;
	}
	return (pipe);
}

t_ast_node	*handle_error(t_ast_node *cmd)
{
	t_ast_node	*error;

	error = create_node(N_ERROR);
	error->child = cmd;
	cmd->parent = error;
	return (error);
}

t_ast_node	*parse_complete_command(t_parser_state *state)
{
	t_ast_node	*command;

	command = parse_command(state);
	if (match(state, PIPE))
		return (handle_pipe_sequence(state, command));
	else if (peek(state).type == EOF_TOKEN)
		return (command);
	else
		return (handle_error(command));
}

void	print_ast(t_ast_node *node, int level)
{
	const char	*data_display;
	int			i;

	if (node == NULL)
		return ;
	i = -1;
	while (++i < level)
		printf("  ");
	if (node->data)
		data_display = node->data;
	else
		data_display = "NULL";
	printf("Node Type: %d | Data: %s\n", node->type, data_display);
	if (node->child)
		print_ast(node->child, level + 2);
	if (node->sibling)
		print_ast(node->sibling, level);
}

t_ast_node	*parse_input(const char *input)
{
	t_token			*tokens;
	t_parser_state	state;
	t_ast_node		*ast;

	tokens = lex(input);
	init_parser_state(&state, tokens);
	ast = parse_complete_command(&state);
	if (LOG_DETAILS)
	{
		printf("\033[1;31m Tokens:::: \033[0m \n");
		log_tokens(tokens);
		printf("\033[1;31m AST:::: \033[0m \n");
		print_ast(ast, 0);
		printf("\033[0;33m Output:::: \033[0m \n");
	}
	destroy_tokens(tokens);
	return (ast);
}
