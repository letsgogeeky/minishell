#include "minishell/minishell.h"
#include "minishell/execution/executor.h"

void	wait_for_children(pid_t last_pid, char **cmds)
{
	int i;

	if (!cmds)
		return ;
	if (str_arr_len(cmds) == 0 || (
		str_arr_len(cmds) == 1 && runs_on_parent(cmds[0])))
		return ;
	signal(SIGINT, SIG_IGN);
	waitpid(last_pid, &g_exit_code, 0);
	if (WIFSIGNALED(g_exit_code))
	{
		g_exit_code = WTERMSIG(g_exit_code);
		if (g_exit_code == SIGINT)
			g_exit_code += 128;
		else
			g_exit_code += 129;
	}
	else
		g_exit_code = WEXITSTATUS(g_exit_code);
	i = 0;
	while (cmds[i])
	{
		wait(NULL);
		i++;
	}
}