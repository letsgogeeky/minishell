#include "minishell/execution/executor.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"
#include "minishell/error.h"

int	exec_cmd(t_minishell *ms, char *cmd);
void	init_fds(t_minishell *ms);
// TODO: change arguments to be the expected structs

void	executor(t_minishell *ms)
{
	pid_t	pid;
	int		i;

	i = 0;
	pid = 1;
	init_fds(ms);
	while (ms->cmds[i])
	{
		// do input redirection
		do_input_redirection(ms->pipe_fd, !i, ms->in_fd);
		// do pipe
		pipe(ms->pipe_fd);
		// check if builtin runs on parent
		// if builtin runs on parent, continue without forking
		if (is_builtin(ms->cmds[i]) && runs_on_parent(ms->cmds[i]))
		{
			ms->cmds[i] = trim_end(trim_start(ms->cmds[i], true), true);
			ms->exit_code = exec_builtin(ms, ms->cmds[i]);
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
			do_output_redirection(ms->pipe_fd, !ms->cmds[i + 1], ms->system_fd[1], ms->out_fd);
			use_child_signals();
			ms->cmds[i] = trim_end(trim_start(ms->cmds[i], true), true);
			exec_cmd(ms, ms->cmds[i]);
		}
		i++;
	}
	restore_io(ms->system_fd, ms->pipe_fd);
	wait_for_children(pid, ms);
}


// TODO: change arguments to be the expected structs
int	exec_cmd(t_minishell *ms, char *cmd)
{
	char	*path;
	char	**parts;

	if (is_builtin(cmd))
	{
		exec_builtin(ms, cmd);
		// TODO: free allocated memory
		exit(ms->exit_code);
	}
	parts = ft_split(cmd, ' ');
	path = get_path(parts[0], ms->envp);
	if (execve(path, parts, ms->envp) == -1)
	{
		free(parts);
		if (!access(path, F_OK) && access(path, X_OK) < 0)
			err(cmd, "Permission denied", 126, ms);
		err(cmd, "command not found", 127, ms);
	}
	return (EXIT_SUCCESS);
}

void	init_fds(t_minishell *ms)
{
	ms->system_fd[0] = dup(STDIN_FILENO);
	ms->system_fd[1] = dup(STDOUT_FILENO);
	ms->pipe_fd[0] = 0;
	ms->pipe_fd[1] = 0;
}