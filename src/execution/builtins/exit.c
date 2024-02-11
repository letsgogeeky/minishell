#include "minishell/error.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"

void builtins_exit(t_minishell *ms)
{
    // TODO: free all memory and exit with relevant exit code.
	clear_history();
    reset_terminos();
	str_arr_free(ms->cmds);
	free_env(ms->envp);
    exit(ms->exit_code);
}
