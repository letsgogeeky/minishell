/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:40:29 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/12 03:49:24 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"

int	is_builtin(char *cmd)
{
	if (!ft_strncmp(cmd, "pwd", 3) || !ft_strncmp(cmd, "env", 3) || \
		!ft_strncmp(cmd, "exit", 4) || !ft_strncmp(cmd, "echo", 4) || \
		!ft_strncmp(cmd, "export", 6) || !ft_strncmp(cmd, "unset", 5) || \
		!ft_strncmp(cmd, "cd", 2))
		return (1);
	return (0);
}

int	runs_on_parent(char *cmd)
{
	if (!ft_strncmp(cmd, "export", 6) || !ft_strncmp(cmd, "unset", 5) || \
		!ft_strncmp(cmd, "exit", 4))
		return (1);
	return (0);
}

int	exec_builtin(t_minishell *ms, char *cmd)
{
	if (!ft_strncmp(cmd, "pwd", 3))
		return (builtins_pwd());
	if (!ft_strncmp(cmd, "env", 3))
		return (builtins_env(ms->envp));
	if (!ft_strncmp(cmd, "exit", 4))
		return (builtins_exit(ms), 0);
	if (!ft_strncmp(cmd, "echo", 4))
		return (builtins_echo(ft_split(cmd, ' ')), 1);
	if (!ft_strncmp(cmd, "export", 6))
		return (builtins_export(ft_split(ft_substr(cmd, 7, ft_strlen(cmd) - 7), ' '), ms), 0);
	if (!ft_strncmp(cmd, "unset", 5))
		return (builtins_unset(ft_split(ft_substr(cmd, 6, ft_strlen(cmd) - 6), ' '), ms));
	if (!ft_strncmp(cmd, "cd", 2))
		return (builtins_cd(ft_split(cmd, ' '), &ms->envp));
	return (0);
}
