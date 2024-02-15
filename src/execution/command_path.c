/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 19:31:03 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/15 23:09:13 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell/execution/executor.h"

static char	*find_path_in_env(char **envp, char *program)
{
	int		idx;
	char	*path;
	char	**env_paths;

	idx = 0;
	while (envp[idx] && ft_strncmp(envp[idx], "PATH=", 5))
		idx++;
	if (!envp[idx])
		return (NULL);
	path = ft_substr(envp[idx], 5, ft_strlen(envp[idx]) - 5);
	env_paths = ft_split(path, ':');
	free(path);
	idx = 0;
	while (env_paths[idx])
	{
		path = ft_strjoin(env_paths[idx], "/");
		path = ft_strjoin_s1_free(path, program);
		if (!access(path, X_OK | F_OK))
			return (path);
		free(path);
		path = NULL;
		idx++;
	}
	str_arr_free(env_paths);
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	*path;

	if (!envp || str_arr_len(envp) == 0)
		return (ft_strjoin_s1_free(ft_strdup("/usr/bin/"), cmd));
	if (ft_strncmp(cmd, "./", 2) == 0)
		return (ft_strdup("/bin/sh"));
	path = find_path_in_env(envp, cmd);
	if (!path)
		return (ft_strdup(cmd));
	return (path);
}
