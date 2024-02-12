/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:49:10 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/12 00:57:42 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"

void	add_or_update_env_variable(char *key, char *value, t_minishell *ms);

int	builtins_export(char **args, t_minishell *ms)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	if (!ms->envp)
		return (print_builtin_error("export", NULL, "no envp"), EXIT_FAILURE);
	i = -1;
	while (args[++i])
	{
		j = 0;
		while (args[i][j] && args[i][j] != '=')
			j++;
		key = ft_substr(args[i], 0, j);
		if (!is_valid_env_name(key))
			return (print_builtin_error(
					"export", key, "not a valid identifier"), \
				free(key), EXIT_FAILURE);
		value = ft_substr(args[i], j + 1, ft_strlen(args[i]) - j - 1);
		add_or_update_env_variable(key, value, ms);
		free(key);
		free(value);
	}
	return (EXIT_SUCCESS);
}

void	add_or_update_env_variable(char *key, char *value, t_minishell *ms)
{
	if (exists_in_env(key, ms->envp) == -1)
		ms->envp = add_to_env(key, value, ms->envp);
	else
		update_env_variable(key, value, &ms->envp);
}
