/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:34:53 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/01/06 21:19:05 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"

void	do_input_redirection(int *pipe_io, bool is_first_cmd)
{
    //  TODO: update to check if input is stdin or file
    if (is_first_cmd)
        return ;
    dup2(pipe_io[0], STDIN_FILENO);
    if (!is_first_cmd)
    {
        close(pipe_io[0]);
        close(pipe_io[1]);
    }
    
}

void	do_output_redirection(int *pipe_io, bool is_last_cmd, int system_out_fd)
{
    if (is_last_cmd)
        dup2(system_out_fd, STDOUT_FILENO);
    else
        dup2(pipe_io[1], STDOUT_FILENO);
    close(pipe_io[1]);
    close(pipe_io[0]);
    close(system_out_fd);
}

void restore_io(int *system_io, int *pipe_io)
{
    dup2(system_io[0], STDIN_FILENO);
    dup2(system_io[1], STDOUT_FILENO);
    close(system_io[0]);
    close(system_io[1]);
    close(pipe_io[1]);
    close(pipe_io[0]);
    
}