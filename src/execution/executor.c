/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:43:22 by ramoussa          #+#    #+#             */
/*   Updated: 2023/12/30 03:17:40 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"

// TODO: change arguments to be the expected structs
int	exec_cmd(char *cmd, char **envp)
{
	char	*path;
	char	**parts;
	pid_t	pid;
	int	status;

	parts = ft_split(cmd, ' ');
	path = get_path(parts[0], envp);
	// TODO: check if EXIT_FAILURE is the right return value
	if (!path)
		return (EXIT_FAILURE);
	printf("path: %s\n", path);
	int i = 0;
	while (parts[i])
	{
		printf("parts[%d]: %s\n", i, parts[i]);
		i++;
	}
	pid = fork();
	if (pid == 0)
		execve(path, parts, envp);
	pid = waitpid(pid, &status, 0);
	return (EXIT_SUCCESS);
}
