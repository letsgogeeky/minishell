
#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdio.h>
# include <stdbool.h>
# include "baselib.h"
# include "minishell/execution/builtins.h"
# include <sys/wait.h>
# include "minishell/minishell.h"

struct s_executor
{
	pid_t	last_pid;
	char	**envp;
	int		pipe_in_fd;
	int		pipe_out_fd;
};

char	*get_path(char *cmd, char **envp);
void	do_input_redirection(t_minishell *ms, bool is_first_cmd, t_ast_node *file);
void	do_output_redirection(t_minishell *ms, bool is_last_cmd, t_ast_node *file);
void	restore_io(int *system_io, int *pipe_io, bool is_empty);
void	executor(t_minishell *ms, int order);
void	execute_ast(t_minishell *ms, t_ast_node *root);
int	count_cmds(t_minishell *ms, t_ast_node *node, bool is_child);
void	wait_for_children(pid_t last_pid, t_minishell *ms);

#endif