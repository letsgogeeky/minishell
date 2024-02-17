/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:34:53 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/18 00:45:23 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"
#include "minishell/minishell.h"

void	init_fds(t_minishell *ms)
{
	ms->system_fd[0] = dup(STDIN_FILENO);
	ms->system_fd[1] = dup(STDOUT_FILENO);
	ms->pipe_fd[0] = 0;
	ms->pipe_fd[1] = 0;
}

void	do_input_redirection(t_minishell *ms, bool is_first_cmd, t_ast_node *file)
{
    if (file != NULL && file->is_heredoc)
		parse_heredoc(ms, file);
    if (file != NULL && file->fd != -1 && file->type == N_INFILE)
    {
        dup2(file->fd, STDIN_FILENO);
        close(file->fd);
    }
    else
        dup2(ms->pipe_fd[0], STDIN_FILENO);
    if (!is_first_cmd)
    {
        close(ms->pipe_fd[0]);
        close(ms->pipe_fd[1]);
    }
}

void	do_output_redirection(t_minishell *ms, bool is_last_cmd, t_ast_node *file)
{
    if (file)
    {
        dup2(file->fd, STDOUT_FILENO);
        close(file->fd);
    }
    else if (is_last_cmd)
        dup2(ms->system_fd[1], STDOUT_FILENO);
    else
        dup2(ms->pipe_fd[1], STDOUT_FILENO);
    close(ms->pipe_fd[1]);
    close(ms->pipe_fd[0]);
    close(ms->system_fd[0]);
}

void restore_io(int *system_io, int *pipe_io, bool is_empty)
{
    dup2(system_io[0], STDIN_FILENO);
    dup2(system_io[1], STDOUT_FILENO);
    close(system_io[0]);
    close(system_io[1]);
	if (is_empty)
		return ;
    close(pipe_io[1]);
    close(pipe_io[0]);
}

