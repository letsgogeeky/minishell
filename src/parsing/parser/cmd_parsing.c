/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 01:49:42 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 02:35:04 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/parser.h"
#include "baselib.h"

t_ast_node	*parse_cmd_suffix(t_parser_state *state)
{
	t_ast_node	*head;
	t_ast_node	*current;
	t_ast_node	*node;

	head = NULL;
	current = NULL;
	node = NULL;
	while (peek(state).type == ASSIGNMENT || is_redirect(state) \
		|| peek(state).type == OPTION || peek(state).type == WORD)
	{
		if (is_redirect(state))
			node = parse_redirect(state);
		else
		{
			node = create_node(determine_node_type(peek(state).type));
			node->data = ft_strdup(peek(state).lexeme);
			consume(state);
		}
		if (!head)
			head = node;
		else
			current->sibling = node;
		current = node;
	}
	return (head);
}

t_ast_node	*parse_cmd_prefix(t_parser_state *state)
{
	t_ast_node	*head;
	t_ast_node	*current;
	t_ast_node	*node;

	head = NULL;
	current = NULL;
	node = NULL;
	while (is_redirect(state) || peek(state).type == ASSIGNMENT)
	{
		if (is_redirect(state))
			node = parse_redirect(state);
		else if (peek(state).type == ASSIGNMENT)
		{
			node = create_node(determine_node_type(peek(state).type));
			node->data = ft_strdup(peek(state).lexeme);
			consume(state);
		}
		if (!head)
			head = node;
		else
			current->sibling = node;
		current = node;
	}
	return (head);
}

t_ast_node	*parse_command(t_parser_state *state)
{
	t_ast_node	*cmd;
	t_ast_node	*last_child;
	t_ast_node	*cmd_suffix;
	t_ast_node	*cmd_prefix;
	t_ast_node	*cmd_word;

	cmd = create_node(N_COMMAND);
	last_child = NULL;
	cmd_suffix = NULL;
	cmd_prefix = NULL;
	cmd_word = NULL;
	if (is_redirect(state) || peek(state).type == ASSIGNMENT)
	{
		cmd_prefix = parse_cmd_prefix(state);
		cmd->child = cmd_prefix;
		last_child = cmd_prefix;
		while (last_child->sibling != NULL)
			last_child = last_child->sibling;
	}
	if (peek(state).type == WORD)
	{
		cmd_word = create_node(N_CMD_WORD);
		cmd_word->data = ft_strdup(peek(state).lexeme);
		consume(state);
		if (last_child)
			last_child->sibling = cmd_word;
		else
			cmd->child = cmd_word;
		last_child = cmd_word;
	}
	if (peek(state).type == ASSIGNMENT || is_redirect(state) \
		|| peek(state).type == WORD)
	{
		cmd_suffix = parse_cmd_suffix(state);
		if (last_child)
			last_child->sibling = cmd_suffix;
		else
			cmd->child = cmd_suffix;
	}
	return (cmd);
}
