#include "minishell/error.h"
#include "minishell/execution/builtins.h"

int builtins_env(char **envp)
{
    int i = 0;
    while (envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    return (EXIT_SUCCESS);
}
