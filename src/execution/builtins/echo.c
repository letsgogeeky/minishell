/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:44:56 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/12 03:50:56 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"

void prettify_str(char *str);

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
		prettify_str(cmds[i]);
		if (cmds[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

void	print_without_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
			printf("%c", str[i]);
		i++;
	}
}

bool is_quoted(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

bool is_wrapped_in_quotes(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\'' || str[i] == '\"')
	{
		while (str[i])
			i++;
		i--;
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
	}
	return (false);
}

void prettify_str(char *str)
{
	int	i;
	bool	is_quoted;

	i = 0;
	is_quoted = is_wrapped_in_quotes(str);
	printf("%d\n", is_quoted);
	if (is_quoted)
		i++;
	while (str[i])
	{
		if (is_quoted && !str[i + 1])
			break;
		if (str[i] == '\\')
		{
			if (str[i + 1] == 'n')
				printf("\n");
			else if (str[i + 1] == 't')
				printf("\t");
			else
				printf("%c", str[i + 1]);
			i++;
		}
		else
			printf("%c", str[i]);
		i++;
	}
}
