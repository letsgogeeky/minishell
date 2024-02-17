/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:42:58 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/17 21:38:07 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"

char	*get_cd_path(char **cmd, char **envp);

int	builtins_cd(char **cmds, t_minishell *ms)
{
	char	*path;
	char	*oldpwd;
	char	*pwd;

	if (!ms->envp)
		return (print_builtin_error("cd", NULL, "no envp"), EXIT_FAILURE);
	path = get_cd_path(cmds, ms->envp);
	if (!path)
		return (EXIT_FAILURE);
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		free(path);
		return (print_builtin_error("cd", cmds[0], strerror(errno)),
			EXIT_FAILURE);
	}
	pwd = getcwd(NULL, 0);
	update_env_variable("OLDPWD", oldpwd, ms);
	update_env_variable("PWD", pwd, ms);
	free(path);
	free(oldpwd);
	free(pwd);
	return (EXIT_SUCCESS);
}

char	*get_cd_path(char **cmd, char **envp)
{
	char	*path;

	if (!cmd[0] || !ft_strncmp(cmd[0], "~", 1) || !ft_strncmp(cmd[0], "~/", 2))
	{
		path = get_env_value("HOME", envp);
		if (!path)
			return (print_builtin_error("cd", "HOME", "not set"), NULL);
	}
	else if (ft_strncmp(cmd[0], "-", 1))
		path = ft_strdup(cmd[0]);
	else
	{
		path = get_env_value("OLDPWD", envp);
		if (!path)
			return (print_builtin_error("cd", "OLDPWD", "not set"), NULL);
	}
	return (path);
}
