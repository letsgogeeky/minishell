#include "minishell/minishell.h"

void    handle_sigint(int sig)
{
    if (sig != SIGINT)
        return ;
    write(STDIN_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_insert_text("");
    rl_redisplay();
}

void    use_signals()
{
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, &handle_sigint);
}