/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 00:11:28 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 22:16:40 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

bool	is_executable_node(t_ast_node *node)
{
	return (node->type == N_CMD_WORD || \
			node->type == N_INFILE || \
			node->type == N_OUTFILE);
}

int	count_cmds(t_minishell *ms, t_ast_node *node, bool is_child)
{
	if (node == NULL)
		return (0);
	if (is_child && is_executable_node(node))
	{
		return (1);
	}
	return (
		count_cmds(ms, node->child, true) + count_cmds(ms, node->sibling, false)
	);
}

char	**siblings_to_array(t_ast_node *node)
{
	int			i;
	char		**arr;
	t_ast_node	*sibling;

	if (!node)
		return (NULL);
	i = 0;
	sibling = node->sibling;
	while (sibling)
	{
		i++;
		sibling = sibling->sibling;
	}
	arr = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	sibling = node->sibling;
	while (sibling)
	{
		arr[i] = ft_strdup(sibling->data);
		i++;
		sibling = sibling->sibling;
	}
	arr[i] = NULL;
	return (arr);
}

bool	has_outfile(t_ast_node *node)
{
	t_ast_node	*sibling;

	sibling = node;
	while (sibling)
	{
		if (sibling->type == N_OUTFILE)
			return (true);
		sibling = sibling->sibling;
	}
	return (false);
}

t_ast_node	*get_outfile_node(t_ast_node *node)
{
	t_ast_node	*sibling;
	t_ast_node	*outfile;

	sibling = node;
	outfile = NULL;
	while (sibling)
	{
		if (sibling->type == N_OUTFILE)
		{
			if (outfile)
				close(outfile->fd);
			outfile = sibling;
		}
		sibling = sibling->sibling;
	}
	return (outfile);
}
