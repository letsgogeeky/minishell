/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 00:11:28 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 02:06:29 by ramoussa         ###   ########.fr       */
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

char	**get_arr_without_last(char **arr)
{
	int		i;
	char	**new_arr;

	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
		i++;
	new_arr = (char **)malloc(sizeof(char *) * i);
	i = 0;
	while (arr[i + 1])
	{
		new_arr[i] = ft_strdup(arr[i]);
		i++;
	}
	new_arr[i] = NULL;
	str_arr_free(arr);
	return (new_arr);
}
