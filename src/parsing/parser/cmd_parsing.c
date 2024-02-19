/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 01:49:42 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 03:50:30 by fvoicu           ###   ########.fr       */
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

t_ast_node	*attach_command_word(t_parser_state *state, \
						t_ast_node *cmd, t_ast_node *prev)
{
	t_ast_node	*cmd_word;

	cmd_word = NULL;
	if (peek(state).type == WORD)
	{
		cmd_word = create_node(N_CMD_WORD);
		cmd_word->data = ft_strdup(peek(state).lexeme);
		consume(state);
		if (prev)
			prev->sibling = cmd_word;
		else
			cmd->child = cmd_word;
		return (cmd_word);
	}
	return (prev);
}

t_ast_node	*parse_command(t_parser_state *state)
{
	t_ast_node	*cmd;
	t_ast_node	*next;
	t_ast_node	*last_child;

	cmd = create_node(N_COMMAND);
	last_child = NULL;
	if (is_redirect(state) || peek(state).type == ASSIGNMENT)
	{
		last_child = parse_cmd_prefix(state);
		cmd->child = last_child;
		while (last_child && last_child->sibling)
			last_child = last_child->sibling;
	}
	last_child = attach_command_word(state, cmd, last_child);
	while (peek(state).type == ASSIGNMENT || \
		is_redirect(state) || peek(state).type == WORD)
	{
		next = parse_cmd_suffix(state);
		if (last_child)
			last_child->sibling = next;
		else if (!cmd->child)
			cmd->child = next;
		last_child = next;
	}
	return (cmd);
}
