#include "minishell/minishell.h"
#include "minishell/execution/executor.h"

void	wait_for_children(pid_t last_pid, t_minishell *ms)
{
	int i;

	if (!ms->count)
		return ;
	if (ms->count == 0 || (
		ms->count == 1 && runs_on_parent(ms->first_cmd)))
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
	while (ms->count > i)
	{
		wait(NULL);
		i++;
	}
}

char	**join_cmd_and_options(char *cmd, char **options)
{
	char	**cmd_and_options;
	int		i;

	i = 0;
	while (options[i])
		i++;
	cmd_and_options = (char **)malloc(sizeof(char *) * (i + 2));
	cmd_and_options[0] = ft_strdup(cmd);
	i = 0;
	while (options[i])
	{
		cmd_and_options[i + 1] = ft_strdup(options[i]);
		i++;
	}
	cmd_and_options[i + 1] = NULL;
	return (cmd_and_options);
}
