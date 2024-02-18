/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 19:30:50 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 19:13:14 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"
#include "minishell/error.h"
#include "minishell/stack.h"

int			exec_cmd(t_minishell *ms, char *cmd, char **options);
void		executor(t_minishell *ms, t_ast_node *node, int order);
void		pre_execute(t_minishell *ms, t_ast_node *node, int order);
char		**join_cmd_and_options(char *cmd, char **options);
void		spawn_process(t_minishell *ms, t_ast_node *node, int order, char *cmd, char **options);

bool	is_collectible_node(t_ast_node *node)
{
	return (node->type == N_CMD_WORD || \
			node->type == N_CMD_PREFIX || \
			node->type == N_CMD_SUFFIX || \
			node->type == N_CMD_PARAM);
}
int	count_options(t_ast_node *node)
{
	int		i;
	t_ast_node	*tmp;
	bool	cmd_encountered;

	cmd_encountered = false;
	i = 0;
	tmp = node;
	while (tmp)
	{
		if (is_collectible_node(tmp) && cmd_encountered)
			i++;
		if (is_collectible_node(tmp) && !cmd_encountered)
			cmd_encountered = true;
		tmp = tmp->sibling;
	}
	return (i);
}
char	**collect_options(t_ast_node *node)
{
	char	**options;
	int		i;
	bool	cmd_encountered;
	t_ast_node	*tmp;

	i = count_options(node);
	tmp = node;
	cmd_encountered = false;
	options = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	tmp = node;
	while (tmp)
	{
		if (is_collectible_node(tmp) && cmd_encountered)
		{
			options[i] = ft_strdup(tmp->data);
			i++;
		}
		if (is_collectible_node(tmp) && !cmd_encountered)
			cmd_encountered = true;
		tmp = tmp->sibling;
	}
	return (options[i] = NULL, options);
}


char	*collect_cmd(t_ast_node *node)
{
	t_ast_node	*tmp;

	tmp = node;
	while (tmp)
	{
		if (is_collectible_node(tmp))
		{
			return (ft_strdup(tmp->data));
		}
		tmp = tmp->sibling;
	}
	return (ft_strdup(""));
}

void	execute_ast(t_minishell *ms, t_ast_node *root)
{
	int				order;
	t_stack_data	top;
	t_ast_node		*node;

	ms->last_pid = 1;
	order = 0;
	stack_push(&ms->stack, root, false);
	while (!stack_is_empty(ms->stack))
	{
		top = stack_pop(&ms->stack);
		node = top.node;
		if (top.is_child && is_executable_node(node))
		{
			pre_execute(ms, node, order);
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

void	pre_execute(t_minishell *ms, t_ast_node *node, int order)
{
	t_ast_node	*tmp;
	bool		has_infile;

	has_infile = false;
	tmp = node;
	while (tmp)
	{
		if (tmp->type == N_INFILE)
		{
			if (has_infile)
				err_io(tmp->data, "Ambiguous input redirections..", 1, ms);
			else
			{
				ms->file_node = tmp;
				do_input_redirection(ms, !order, tmp);
				has_infile = true;
			}
		}
		tmp = tmp->sibling;
	}
	if (!has_infile)
		do_input_redirection(ms, !order, NULL);
	if (order == 0)
		ms->first_cmd = collect_cmd(node);
}

void	executor(t_minishell *ms, t_ast_node *node, int order)
{
	char	*cmd;
	char	**options;

	options = collect_options(node);
	cmd = collect_cmd(node);
	if (!node)
		return ;
	pipe(ms->pipe_fd);
	if (is_builtin(node->data) && runs_on_parent(node->data))
	{
		ms->exit_code = exec_builtin(ms, cmd, options);
		str_arr_free(options);
		free(cmd);
		return ;
	}
	spawn_process(ms, node, order, cmd, options);
	str_arr_free(options);
	free(cmd);
}

void	spawn_process(t_minishell *ms, t_ast_node *node, int order, char *cmd, char **options)
{
	bool	infile_error;

	infile_error = false;
	if (ms->file_node && ms->file_node->type == N_INFILE && ms->file_node->fd == -1)
		infile_error = true;
	ms->file_node = NULL;
	ms->last_pid = fork();
	if (ms->last_pid == 0)
	{
		if (order + 1 == ms->count && has_outfile(node))
			ms->file_node = get_outfile_node(node);
		do_output_redirection(ms, order + 1 == ms->count, ms->file_node);
		if (infile_error || (ms->file_node != NULL && ms->file_node->fd == -1))
			exit(1);
		use_child_signals();
		if (ft_strlen(cmd) > 0)
		{
			exec_cmd(ms, cmd, options);
			free(cmd);
		}
		else
			exit(0);
	}
}

int	exec_cmd(t_minishell *ms, char *cmd, char **options)
{
	char	*path;
	char	**parts;

	if (is_builtin(cmd))
	{
		ms->exit_code = exec_builtin(ms, cmd, options);
		str_arr_free(options);
		exit(ms->exit_code);
	}
	parts = join_cmd_and_options(cmd, options);
	str_arr_free(options);
	path = get_path(cmd, ms->envp);
	if (execve(path, parts, ms->envp) == -1)
	{
		str_arr_free(parts);
		if (!access(path, F_OK) && access(path, X_OK) < 0)
			return (free(path), err(cmd, "Permission denied", 126, ms), \
				EXIT_FAILURE);
		return (free(path), err(cmd, "command not found", 127, ms), \
			EXIT_FAILURE);
	}
	return (free(path), free(cmd), str_arr_free(parts), EXIT_SUCCESS);
}
