#include "minishell/error.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"

void builtins_exit(char **cmds, char **envp)
{
    // TODO: free all memory and exit with relevant exit code.
	clear_history();
    reset_terminos();
	str_arr_free(cmds);
	free_env(envp);
    exit(g_exit_code);
}
