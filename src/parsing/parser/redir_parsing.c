/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 01:52:29 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 02:47:18 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/parser.h"
#include "baselib.h"

bool	is_redirect(t_parser_state *state)
{
	t_token_type	type;

	type = peek(state).type;
	return (type == GREAT || type == LESS \
		|| type == DLESS || type == DGREAT);
}

t_ast_node	*parse_redirect(t_parser_state *state)
{
	t_ast_node		*node;
	t_token_type	type;
	t_token			file;

	type = peek(state).type;
	consume(state);
	file = consume(state);
	node = create_node(determine_node_type(type));
	node->data = ft_strdup(file.lexeme);
	if (type == GREAT)
		node->fd = open_file(file.lexeme, O_TRUNC);
	else if (type == DGREAT)
		node->fd = open_file(file.lexeme, O_APPEND);
	else if (type == LESS)
		node->fd = open_file(file.lexeme, O_RDONLY);
	else if (type == DLESS)
		node->is_heredoc = true;
	return (node);
}
