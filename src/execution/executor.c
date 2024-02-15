#include "minishell/execution/executor.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"
#include "minishell/error.h"
#include "minishell/stack.h"

int		exec_cmd(t_minishell *ms, char *cmd, char **options);
void	executor(t_minishell *ms, t_ast_node *node, int order);
t_ast_node	*pre_execute(t_minishell *ms, t_ast_node *node, int order);

void execute_ast(t_minishell *ms, t_ast_node *root)
{
	int	order;
	t_stack_data top;
	t_ast_node *node;

	ms->last_pid = 1;
	order = 0;
    stack_push(&ms->stack, root, false);
    while (!stack_is_empty(ms->stack))
	{
        top = stack_pop(&ms->stack);
        node = top.node;
        if (node == NULL) continue;
        if (top.is_child && is_executable_node(node))
		{
			node = pre_execute(ms, node, order);
			executor(ms, node, order);
			order++;
        }
        if (node->sibling)
            stack_push(&(ms->stack), node->sibling, false);
        if (node->child)
            stack_push(&(ms->stack), node->child, true);
    }
	restore_io(ms->system_fd, ms->pipe_fd, ms->count == 0);
	wait_for_children(ms->last_pid, ms);
}

t_ast_node	*pre_execute(t_minishell *ms, t_ast_node *node, int order)
{
	if (order == 0 && node->type == N_INFILE)
	{
		ms->file_node = node;
		node = node->sibling;
	}
	if (order == 0)
		ms->first_cmd = ft_strdup(node->data);
	return (node);
}

void	executor(t_minishell *ms, t_ast_node *node, int order)
{
	char	**siblings;

	siblings = siblings_to_array(node);
	do_input_redirection(ms, !order, ms->file_node);
	ms->file_node = NULL;
	if (order + 1 == ms->count && get_last_sibiling(node)->type == N_INFILE)
	{
		ms->file_node = get_last_sibiling(node);
		siblings = get_arr_without_last(siblings);
	}
	pipe(ms->pipe_fd);
	if (is_builtin(node->data) && runs_on_parent(node->data))
	{
		ms->exit_code = exec_builtin(ms, node->data, siblings);
		return ;
	}
	ms->last_pid = fork();
	if (ms->last_pid == 0)
	{
		do_output_redirection(ms, order + 1 == ms->count, ms->file_node);
		use_child_signals();
		exec_cmd(ms, node->data, siblings);
	}
}

int	exec_cmd(t_minishell *ms, char *cmd, char **options)
{
	char	*path;
	char	**parts;
	
	if (is_builtin(cmd))
	{
		exec_builtin(ms, cmd, options);
		// TODO: free allocated memory
		exit(ms->exit_code);
	}
	parts = join_cmd_and_options(cmd, options);
	path = get_path(cmd, ms->envp);
	if (execve(path, parts, ms->envp) == -1)
	{
		str_arr_free(parts);
		str_arr_free(options);
		if (!access(path, F_OK) && access(path, X_OK) < 0)
			return (free(path), err(cmd, "Permission denied", 126, ms), EXIT_FAILURE);
		return (free(path), err(cmd, "command not found", 127, ms), EXIT_FAILURE);
	}
	return (free(path), str_arr_free(parts), str_arr_free(options), EXIT_SUCCESS);
}
