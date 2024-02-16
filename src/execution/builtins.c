/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:40:29 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/16 02:53:23 by ramoussa         ###   ########.fr       */
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
		!ft_strncmp(cmd, "exit", 4) || !ft_strncmp(cmd, "cd", 2))
		return (1);
	return (0);
}

int	exec_builtin(t_minishell *ms, char *cmd, char **options)
{
	if (!ft_strncmp(cmd, "pwd", 3))
		return (builtins_pwd());
	if (!ft_strncmp(cmd, "env", 3))
		return (builtins_env(ms->envp));
	if (!ft_strncmp(cmd, "exit", 4))
		return (builtins_exit(ms, options), 0);
	if (!ft_strncmp(cmd, "echo", 4))
		return (builtins_echo(ms, options), 1);
	if (!ft_strncmp(cmd, "export", 6))
		return (builtins_export(options, ms), 0);
	if (!ft_strncmp(cmd, "unset", 5))
		return (builtins_unset(options, ms));
	if (!ft_strncmp(cmd, "cd", 2))
		return (builtins_cd(options, ms));
	return (0);
}
