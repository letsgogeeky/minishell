#include "minishell/error.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"

int builtins_exit(char **cmds, char **envp)
{
    (void)envp;
    (void)cmds;
    // TODO: free all memory and exit with relevant exit code.
    reset_terminos();
    exit(EXIT_SUCCESS);
    return (EXIT_SUCCESS);
}