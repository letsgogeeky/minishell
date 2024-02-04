/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 19:33:40 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/03 20:13:00 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdio.h>
# include <stdbool.h>
# include "baselib.h"

struct s_executor
{
	pid_t	last_pid;
	char	**envp;
	int		pipe_in_fd;
	int		pipe_out_fd;
};

char	*get_path(char *cmd, char **envp);
void	do_input_redirection(int *pipe_io, bool is_first_cmd);
void	do_output_redirection(int *pipe_io, bool is_last_cmd, int system_out_fd);
void	restore_io(int *system_io, int *pipe_io);
void	executor(char **cmds, char ***envp);

#endif