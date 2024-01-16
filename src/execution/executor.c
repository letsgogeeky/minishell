/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:43:22 by ramoussa          #+#    #+#             */
/*   Updated: 2024/01/13 19:50:27 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"

int	exec_cmd(char *cmd, char **envp);
// TODO: change arguments to be the expected structs
void	executor(char **cmds, char **envp)
{
	int		pipe_io[2];
	int		system_io[2];
	pid_t	pid;
	int		i;
	int		status;

	i = 0;
	system_io[0] = dup(STDIN_FILENO);
	system_io[1] = dup(STDOUT_FILENO);
	while (cmds[i])
	{
		// do input redirection
		do_input_redirection(pipe_io, !i);
		// do pipe
		pipe(pipe_io);
		// check if builtin runs on parent
		// if builtin runs on parent, continue without forking
		if (is_builtin(cmds[i]) && runs_on_parent(cmds[i]))
		{
			exec_builtin(cmds[i], envp);
			i++;
			continue ;
		}
		// else do fork
		pid = fork();
		// do redirection
		// check pid = 0
		// call exec_cmd
		if (pid == 0)
		{
			do_output_redirection(pipe_io, !cmds[i + 1], system_io[1]);
			use_child_signals();
			exec_cmd(cmds[i], envp);
		}
		i++;
	}
	// wait for all children
	// restore io
	restore_io(system_io, pipe_io);
	pid = waitpid(pid, &status, 0);
}

// TODO: change arguments to be the expected structs
int	exec_cmd(char *cmd, char **envp)
{
	char	*path;
	char	**parts;

	// TODO: check if builtin
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
	execve(path, parts, envp);
	return (EXIT_SUCCESS);
}
