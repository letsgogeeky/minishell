/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:44:56 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/16 03:52:17 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"
#include "minishell/execution/executor.h"

void prettify_str(t_minishell *ms, char *str);

void	builtins_echo(t_minishell *ms, char **cmds)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;
	if (cmds[i] && cmds[i][0] == '-')
	{
		if (!ft_strncmp(cmds[i], "-n", 2))
			newline = 0;
		i++;
	}
	while (cmds[i])
	{
		prettify_str(ms, cmds[i]);
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
		if (i && (str[i] == '\'' || str[i] == '\"'))
			return (true);
	}
	return (false);
}

void	print_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
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

char	*do_quote_magic(t_minishell *ms, char *str)
{
	int		i;
	bool	is_quoted;
	bool	is_wrapped;
	bool	had_quote;
	bool	is_single;
	
	is_quoted = false;
	is_wrapped = false;
	had_quote = false;
	is_single = false;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\\')
			i++;
		else if (str[i] == '\'' || str[i] == '\"')
		{
			is_wrapped = true;
			had_quote = true;
			if (str[i] == '\'')
				is_single = true;
			is_quoted = !is_quoted;
			if (is_quoted)
				is_wrapped = false;				
		}
	}
	if (had_quote && !is_wrapped)
	{
		if (is_single)
			return (parse_unclosed_quote(ms, "\'", str));
		else
			return (parse_unclosed_quote(ms, "\"", str));
	}
	return (ft_strdup(str));
}

void prettify_str(t_minishell *ms, char *str)
{
	int	i;
	char	*doc;
	
	doc = do_quote_magic(ms, str);
	i = -1;
	while (doc[++i])
	{
		if (doc[i] == '\\')
		{
			if (doc[i + 1] == 'n')
				printf("\n");
			else if (doc[i + 1] == 't')
				printf("\t");
			else
				printf("%c", doc[i + 1]);
			i++;
		}
		else if (doc[i] == '\'' || doc[i] == '\"')
			continue ;
		else
			printf("%c", doc[i]);
	}
	free(doc);
}
