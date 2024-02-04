#include "minishell/execution/builtins.h"
#include "minishell/error.h"
#include "minishell/minishell.h"

int builtins_unset(char **args, char ***envp)
{
    int i;

    if (!envp)
        return (print_builtin_error("unset", NULL, "no envp"), EXIT_FAILURE);
    i = 0;
    while (args[i])
    {
        if (!is_valid_env_name(args[i]))
            return (print_builtin_error("unset", args[i], "not a valid identifier"), EXIT_FAILURE);
        if (exists_in_env(args[i], *envp) != -1)
            *envp = remove_from_env(args[i], *envp);
        i++;
    }
    return (EXIT_SUCCESS);
}
