/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 19:31:18 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/19 03:45:33 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

int	exists_in_env(char *key, char **envp)
{
	int	i;
	int	j;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] == key[j])
			++j;
		if (envp[i][j] == '=' && !key[j])
			return (i);
		++i;
	}
	return (-1);
}

int	update_env_variable(char *key, char *value, t_minishell *ms)
{
	int		i;
	char	*tmp;
	char	*dequoted;

	i = exists_in_env(key, ms->envp);
	if (i == -1)
		return (-1);
	free(ms->envp[i]);
	tmp = ft_strjoin(key, "=");
	dequoted = get_dequoted_value(value);
	ms->envp[i] = ft_strjoin(tmp, dequoted);
	free(dequoted);
	free(tmp);
	return (EXIT_SUCCESS);
}

char	**add_to_env(char *key, char *value, char **envp)
{
	int		i;
	char	**new_envp;
	char	*dequoted;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	i = -1;
	while (envp[++i])
		new_envp[i] = envp[i];
	key = ft_strjoin(key, "=");
	dequoted = get_dequoted_value(value);
	new_envp[i] = ft_strjoin(key, dequoted);
	free(dequoted);
	new_envp[i + 1] = NULL;
	free(key);
	free(envp);
	return (new_envp);
}

char	**remove_from_env(char *key, char **envp)
{
	int		i;
	int		j;
	char	**new_envp;

	i = exists_in_env(key, envp);
	if (i == -1)
		return (envp);
	j = 0;
	while (envp[j])
		++j;
	new_envp = malloc(sizeof(char *) * j);
	j = 0;
	while (envp[j])
	{
		if (j < i)
			new_envp[j] = envp[j];
		else if (j > i)
			new_envp[j - 1] = envp[j];
		else
			free(envp[j]);
		++j;
	}
	new_envp[j - 1] = NULL;
	free(envp);
	return (new_envp);
}

int	is_valid_env_name(char *name)
{
	int	i;

	i = 0;
	if (!ft_isalpha(name[i]) && name[i] != '_')
		return (0);
	++i;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
