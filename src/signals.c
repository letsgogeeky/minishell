#include "minishell/minishell.h"

void    handle_sigint(int sig)
{
    if (sig != SIGINT)
        return ;
    write(STDIN_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void	update_terminos(void)
{
	struct termios	settings;

	tcgetattr(STDIN_FILENO, &settings);
	settings.c_lflag &= ~(ECHOCTL | IEXTEN);
	tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

void    reset_terminos(void)
{
    struct termios	settings;

    tcgetattr(STDIN_FILENO, &settings);
    settings.c_lflag |= (ECHOCTL | IEXTEN);
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
}

void    use_parent_signals()
{
    // prevent shell from exiting on ctrl+\ (SIGQUIT)
    signal(SIGQUIT, SIG_IGN);
    // handle ctrl+c
    signal(SIGINT, &handle_sigint);
    update_terminos();
}

void    use_child_signals()
{
    // use default behavior for ctrl+\ (SIGQUIT)
    signal(SIGQUIT, SIG_DFL);
    // use default behavior for ctrl+c
    signal(SIGINT, SIG_DFL);
}
