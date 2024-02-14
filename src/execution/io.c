/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:34:53 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/14 03:03:42 by ramoussa         ###   ########.fr       */
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
    //  TODO: update to check if input is stdin or file
    if (is_first_cmd && file)
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
    if (is_last_cmd && file)
    {
		printf("file: %s\n", file->data);
		printf("file type: %d\n", file->type);
		printf("file fd: %d\n", file->fd);
        ft_putendl_fd("here", STDERR_FILENO);
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

