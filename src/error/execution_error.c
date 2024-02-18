/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:44:08 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/18 22:09:36 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/error.h"
#include "baselib.h"

void	print_builtin_error(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
	}
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_execve_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	err_io(char *cmd, char *msg, int exit_code, t_minishell *ms)
{
	print_builtin_error(cmd, NULL, msg);
	ms->exit_code = exit_code;
}

void	err(char *cmd, char *msg, int exit_code, t_minishell *ms)
{
	print_execve_error(cmd, msg);
	ms->exit_code = exit_code;
	free(cmd);
	exit(ms->exit_code);
}
