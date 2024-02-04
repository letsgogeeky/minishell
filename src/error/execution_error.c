/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:44:08 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/04 18:38:20 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/error.h"
#include "baselib.h"

void    print_builtin_error(char *cmd, char *arg, char *msg)
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

void   print_execve_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	err(char *cmd, char *msg, int exit_code)
{
	print_execve_error(cmd, msg);
	g_exit_code = exit_code;
	exit(exit_code);
}
