/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 21:17:15 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 22:20:06 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"

char	**join_cmd_and_options(char *cmd, char **options)
{
	char	**cmd_and_options;
	int		i;

	i = 0;
	while (options[i])
		++i;
	cmd_and_options = (char **)malloc(sizeof(char *) * (i + 2));
	cmd_and_options[0] = ft_strdup(cmd);
	i = -1;
	while (options[++i])
		cmd_and_options[i + 1] = ft_strdup(options[i]);
	cmd_and_options[i + 1] = NULL;
	return (cmd_and_options);
}

char	**get_arr_without_last(char **arr)
{
	int		i;
	char	**new_arr;

	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
		++i;
	new_arr = (char **)malloc(sizeof(char *) * i);
	i = 0;
	while (arr[i + 1])
	{
		new_arr[i] = ft_strdup(arr[i]);
		++i;
	}
	new_arr[i] = NULL;
	str_arr_free(arr);
	return (new_arr);
}
