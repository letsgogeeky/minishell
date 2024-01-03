/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 19:33:40 by ramoussa          #+#    #+#             */
/*   Updated: 2023/12/30 00:37:32 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdio.h>
# include "baselib.h"

struct s_executor
{
	pid_t	last_pid;
	char	**envp;
	int		pipe_in_fd;
	int		pipe_out_fd;
};

char	*get_path(char *cmd, char **envp);
int		exec_cmd(char *cmd, char **envp);

#endif