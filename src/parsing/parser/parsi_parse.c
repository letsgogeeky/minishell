/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsi_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 02:37:09 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 02:54:03 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/parser.h"
#include "baselib.h"

t_ast_node	*parse_complete_command(t_parser_state *state)
{
	t_ast_node	*command;
	t_ast_node	*pipe;
	t_ast_node	*next_cmd;
	t_ast_node	*error;

	command = parse_command(state);
	if (match(state, PIPE))
	{
		pipe = create_node(N_PIPE);
		next_cmd = parse_complete_command(state);
		pipe->child = command;
		command->parent = pipe;
		if (next_cmd)
		{
			next_cmd->parent = pipe;
			command->sibling = next_cmd;
		}
		return (pipe);
	}
	else if (peek(state).type == EOF_TOKEN)
		return (command);
	else
	{
		error = create_node(N_ERROR);
		error->child = command;
		command->parent = error;
		return (error);
	}
}

void	print_ast(t_ast_node *node, int level)
{
	int	i;

	if (node == NULL)
		return ;
	i = -1;
	while (++i < level)
		printf("  ");
	printf("%d: %s\n", node->type, node->data ? node->data : "NULL");
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
