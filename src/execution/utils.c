#include "minishell/minishell.h"
#include "minishell/execution/executor.h"

void	wait_for_children(pid_t last_pid, t_minishell *ms)
{
	int i;

	if (!ms->cmds)
		return ;
	if (str_arr_len(ms->cmds) == 0 || (
		str_arr_len(ms->cmds) == 1 && runs_on_parent(ms->cmds[0])))
		return ;
	signal(SIGINT, SIG_IGN);
	waitpid(last_pid, &ms->exit_code, 0);
	if (WIFSIGNALED(ms->exit_code))
	{
		ms->exit_code = WTERMSIG(ms->exit_code);
		if (ms->exit_code == SIGINT)
			ms->exit_code += 128;
		else
			ms->exit_code += 129;
	}
	else
		ms->exit_code = WEXITSTATUS(ms->exit_code);
	i = 0;
	while (ms->cmds[i])
	{
		wait(NULL);
		i++;
	}
}