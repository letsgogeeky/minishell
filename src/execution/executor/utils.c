/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 21:17:15 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/17 21:17:30 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"

char	**join_cmd_and_options(char *cmd, char **options)
{
	char	**cmd_and_options;
	int		i;

	i = 0;
	while (options[i])
		i++;
	cmd_and_options = (char **)malloc(sizeof(char *) * (i + 2));
	cmd_and_options[0] = ft_strdup(cmd);
	i = 0;
	while (options[i])
	{
		cmd_and_options[i + 1] = ft_strdup(options[i]);
		i++;
	}
	cmd_and_options[i + 1] = NULL;
	return (cmd_and_options);
}
