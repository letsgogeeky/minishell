#include "minishell/execution/builtins.h"

int is_builtin(char *cmd)
{
    if (!ft_strncmp(cmd, "pwd", 3) || !ft_strncmp(cmd, "env", 3) ||
        !ft_strncmp(cmd, "exit", 4) || !ft_strncmp(cmd, "echo", 4) ||
        !ft_strncmp(cmd, "export", 6) || !ft_strncmp(cmd, "unset", 5) ||
		!ft_strncmp(cmd, "cd", 2))
        return (1);
    return (0);
}

int runs_on_parent(char *cmd)
{
    if (!ft_strncmp(cmd, "export", 6) || !ft_strncmp(cmd, "unset", 5) ||
		!ft_strncmp(cmd, "cd", 2) || !ft_strncmp(cmd, "exit", 4))
        return (1);
    return (0);
}

int exec_builtin(char **cmds, char *cmd, char ***envp)
{
    if (!ft_strncmp(cmd, "pwd", 3))
        return (builtins_pwd());
    if (!ft_strncmp(cmd, "env", 3))
        return (builtins_env(*envp));
    if (!ft_strncmp(cmd, "exit", 4))
        return (builtins_exit(cmds, *envp), 0);
    if (!ft_strncmp(cmd, "echo", 4))
        return (builtins_echo(ft_split(cmd, ' '))); // TODO: update this to use commands tree
    if (!ft_strncmp(cmd, "export", 6))
        return (builtins_export(ft_split(ft_substr(cmd, 7, ft_strlen(cmd) - 7), ' '), envp));
    if (!ft_strncmp(cmd, "unset", 5))
        return (builtins_unset(ft_split(ft_substr(cmd, 6, ft_strlen(cmd) - 6), ' '), envp));
    return (EXIT_FAILURE);
}