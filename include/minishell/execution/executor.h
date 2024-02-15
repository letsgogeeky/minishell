
#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdio.h>
# include <stdbool.h>
# include "baselib.h"
# include "minishell/execution/builtins.h"
# include <sys/wait.h>
# include "minishell/minishell.h"


char		*get_path(char *cmd, char **envp);
void		do_input_redirection(t_minishell *ms, bool is_first_cmd, t_ast_node *file);
void		do_output_redirection(t_minishell *ms, bool is_last_cmd, t_ast_node *file);
void		restore_io(int *system_io, int *pipe_io, bool is_empty);
void		init_fds(t_minishell *ms);
void		execute_ast(t_minishell *ms, t_ast_node *root);
int			count_cmds(t_minishell *ms, t_ast_node *node, bool is_child);
void		wait_for_children(pid_t last_pid, t_minishell *ms);
char		**join_cmd_and_options(char *cmd, char **options);

bool		is_executable_node(t_ast_node *node);
char		**siblings_to_array(t_ast_node *node);
t_ast_node	*get_last_sibiling(t_ast_node *node);
char		**get_arr_without_last(char **arr);
int			count_cmds(t_minishell *ms, t_ast_node *node, bool is_child);
int			parse_heredoc(t_minishell *ms, t_ast_node *node);

#endif