/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_generation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 01:45:40 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 02:28:30 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/parser.h"
#include "baselib.h"

t_ast_node	*create_node(t_ast_node_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	node->type = type;
	node->parent = NULL;
	node->child = NULL;
	node->sibling = NULL;
	node->data = NULL;
	node->is_heredoc = false;
	node->fd = -1;
	return (node);
}

char	*node_type_to_string(t_ast_node_type type)
{
	if (type == N_COMMAND)
		return ("N_COMMAND");
	else if (type == N_PIPE)
		return ("N_PIPE");
	else if (type == N_CMD_WORD)
		return ("N_CMD_WORD");
	else if (type == N_CMD_PREFIX)
		return ("N_CMD_PREFIX");
	else if (type == N_CMD_SUFFIX)
		return ("N_CMD_SUFFIX");
	else if (type == N_CMD_PARAM)
		return ("N_CMD_PARAM");
	else if (type == N_INFILE)
		return ("N_INFILE");
	else if (type == N_OUTFILE)
		return ("N_OUTFILE");
	else if (type == N_HEREDOC)
		return ("N_HEREDOC");
	else if (type == N_ERROR)
		return ("N_ERROR");
	else
		return ("Unknown node type");
}

t_ast_node_type	determine_node_type(t_token_type type)
{
	if (type == WORD)
		return (N_CMD_WORD);
	else if (type == OPTION)
		return (N_CMD_PARAM);
	else if (type == ASSIGNMENT)
		return (N_CMD_PARAM);
	else if (type == LESS || type == DLESS)
		return (N_INFILE);
	else if (type == GREAT || type == DGREAT)
		return (N_OUTFILE);
	else
		return (printf("Unknown token type encountered.\n"), \
			N_ERROR);
}

void	destroy_ast(t_ast_node *node)
{
	if (!node)
		return ;
	destroy_ast(node->child);
	destroy_ast(node->sibling);
	if (LOG_DETAILS)
		printf("Destroying node: %s of type: %s\n", \
			node->data, node_type_to_string(node->type));
	if (node->data)
		free(node->data);
	free(node);
}
