/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_analysis_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 22:24:37 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/18 22:32:52 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"
#include "minishell/error.h"
#include "minishell/stack.h"

bool	is_collectible_node(t_ast_node *node)
{
	return (node->type == N_CMD_WORD || \
			node->type == N_CMD_PREFIX || \
			node->type == N_CMD_SUFFIX || \
			node->type == N_CMD_PARAM);
}

int	count_options(t_ast_node *node)
{
	int			i;
	t_ast_node	*tmp;
	bool		cmd_encountered;

	cmd_encountered = false;
	i = 0;
	tmp = node;
	while (tmp)
	{
		if (is_collectible_node(tmp) && cmd_encountered)
			i++;
		if (is_collectible_node(tmp) && !cmd_encountered)
			cmd_encountered = true;
		tmp = tmp->sibling;
	}
	return (i);
}

char	**collect_options(t_ast_node *node)
{
	char		**options;
	int			i;
	bool		cmd_encountered;
	t_ast_node	*tmp;

	i = count_options(node);
	tmp = node;
	cmd_encountered = false;
	options = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	tmp = node;
	while (tmp)
	{
		if (is_collectible_node(tmp) && cmd_encountered)
		{
			options[i] = ft_strdup(tmp->data);
			i++;
		}
		if (is_collectible_node(tmp) && !cmd_encountered)
			cmd_encountered = true;
		tmp = tmp->sibling;
	}
	return (options[i] = NULL, options);
}

char	*collect_cmd(t_ast_node *node)
{
	t_ast_node	*tmp;

	tmp = node;
	while (tmp)
	{
		if (is_collectible_node(tmp))
		{
			return (ft_strdup(tmp->data));
		}
		tmp = tmp->sibling;
	}
	return (ft_strdup(""));
}
