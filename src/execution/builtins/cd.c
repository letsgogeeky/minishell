#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"

char	*get_cd_path(char **cmd, char ***envp);

int	builtins_cd(char **cmds, char ***envp)
{
	char	*path;
	char	*oldpwd;
	char	*pwd;

	if (!envp)
		return (print_builtin_error("cd", NULL, "no envp"), EXIT_FAILURE);
	
	path = get_cd_path(cmds, envp);
	if (!path)
		return (EXIT_FAILURE);
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		free(path);
		printf("errno: %d\n", errno);
		return (print_builtin_error("cd", cmds[1], strerror(errno)), EXIT_FAILURE);
	}
	pwd = getcwd(NULL, 0);
	update_env_variable("OLDPWD", oldpwd, envp);
	update_env_variable("PWD", pwd, envp);
	free(path);
	free(oldpwd);
	free(pwd);
	return (EXIT_SUCCESS);
}

char	*get_cd_path(char **cmd, char ***envp)
{
	char *path;

	if (!cmd[1])
	{
		path = get_env_value("HOME", *envp);
		if (!path)
			return (print_builtin_error("cd", "HOME", "not set"), NULL);
	}
	else if (ft_strncmp(cmd[1], "-", 1))
		path = ft_strdup(cmd[1]);
	else
	{
		path = get_env_value("OLDPWD", *envp);
		if (!path)
			return (print_builtin_error("cd", "OLDPWD", "not set"), NULL);
	}
	return (path);
}
