#include "minishell/execution/builtins.h"

int is_builtin(char *cmd)
{
    if (!ft_strncmp(cmd, "pwd", 3) || !ft_strncmp(cmd, "env", 3))
        return (1);
    return (0);
}

int runs_on_parent(char *cmd)
{
    if (!ft_strncmp(cmd, "pwd", 3) || !ft_strncmp(cmd, "env", 3))
        return (1);
    return (0);
}

int exec_builtin(char *cmd, char **envp)
{
    if (!ft_strncmp(cmd, "pwd", 3))
        return (builtins_pwd());
    if (!ft_strncmp(cmd, "env", 3))
        return (builtins_env(envp));
    return (EXIT_FAILURE);
}