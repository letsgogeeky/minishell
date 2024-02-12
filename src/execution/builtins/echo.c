/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:44:56 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/12 00:46:52 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"

void	builtins_echo(char **cmds)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmds[1] && !ft_strncmp(cmds[1], "-n", 2))
	{
		newline = 0;
		i++;
	}
	while (cmds[i])
	{
		printf("%s", cmds[i]);
		if (cmds[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
