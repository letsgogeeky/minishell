/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:44:56 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 22:13:32 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"
#include "minishell/execution/executor.h"

void	prettify_str(t_minishell *ms, char *str);
char	*get_final_str(t_minishell *ms, char *str, t_quote_state qs);
char	*do_quote_magic(t_minishell *ms, char *str);

int	builtins_echo(t_minishell *ms, char **cmds)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;
	if (cmds[i] && cmds[i][0] == '-')
	{
		if (!ft_strncmp(cmds[i], "-n", 2))
			newline = 0;
		++i;
	}
	while (cmds[i])
	{
		prettify_str(ms, cmds[i]);
		if (cmds[i + 1])
			printf(" ");
		++i;
	}
	if (newline)
		printf("\n");
	return (EXIT_SUCCESS);
}

void	prettify_str(t_minishell *ms, char *str)
{
	int		i;
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

char	*do_quote_magic(t_minishell *ms, char *str)
{
	int				i;
	t_quote_state	qs;

	qs = (t_quote_state){false, false, false, false};
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\\')
			i++;
		else if (str[i] == '\'' || str[i] == '\"')
		{
			if (qs.had_quote && qs.is_single && str[i] == '\"')
				continue ;
			if (qs.had_quote && !qs.is_single && str[i] == '\'')
				continue ;
			qs.is_wrapped = true;
			qs.had_quote = true;
			if (str[i] == '\'')
				qs.is_single = true;
			qs.is_quoted = !qs.is_quoted;
			if (qs.is_quoted)
				qs.is_wrapped = false;
		}
	}
	return (get_final_str(ms, str, qs));
}

char	*get_final_str(t_minishell *ms, char *str, t_quote_state qs)
{
	if (qs.is_quoted)
	{
		if (qs.is_single)
			return (parse_unclosed_quote(ms, "\'", str));
		else
			return (parse_unclosed_quote(ms, "\"", str));
	}
	return (ft_strdup(str));
}
