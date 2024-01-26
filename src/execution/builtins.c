#include "minishell/execution/builtins.h"

int is_builtin(char *cmd)
{
    if (!ft_strncmp(cmd, "pwd", 3) || !ft_strncmp(cmd, "env", 3) ||
        !ft_strncmp(cmd, "exit", 4))
        return (1);
    return (0);
}

int runs_on_parent(char *cmd)
{
    if (!ft_strncmp(cmd, "pwd", 3) || !ft_strncmp(cmd, "env", 3) ||
        !ft_strncmp(cmd, "exit", 4))
        return (1);
    return (0);
}

int exec_builtin(char **cmds, char *cmd, char **envp)
{
    if (!ft_strncmp(cmd, "pwd", 3))
        return (builtins_pwd());
    if (!ft_strncmp(cmd, "env", 3))
        return (builtins_env(envp));
    if (!ft_strncmp(cmd, "exit", 4))
        return (builtins_exit(cmds, envp));
    return (EXIT_FAILURE);
}