#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/execution/executor.h"

int g_exit_code = 0;
static int interactive_mode(char **envp)
{
    char    *line;
    char    **cmds;
    // int     out_file_fd;
    // int     in_fd;
    // bool    is_append;

    line = readline("massiveshell$ ");
    while (line && line[0] != EOF)
    {
        use_parent_signals();
        if (line && line[0])
        {
            // TODO: call parser here
            // is_append = false;
            // // temporary solution for output file until is handled by parser
            // if (is_append)
            // {
            //     // for >> append redirection. append is slower to flush data to file
            //     // because we're not allowed to use fflush
            //     out_file_fd = open_file("./out.txt", O_APPEND);
            // }
            // else
            // {
            //     // for > truncate redirection, it takes effect immedicately
            //     out_file_fd = open_file("./out.txt", O_TRUNC);
            // }
            // in_fd = open_file("./in.txt", O_RDONLY);
            // printf("file fd: %d\n", out_file_fd);
            // TODO: add to history and do execution magic and return exit code after
            // builtins_pwd();
            cmds = ft_split(line, '|');
			add_history(line);
            reset_terminos();
            expand(&cmds, &envp);
            int i = 0;
            for (i = 0; cmds[i]; i++)
            {
                printf("cmds[%d]: %s\n", i, cmds[i]);
            }
			executor(cmds, &envp, -1, -1); // TODO: make this proper executor
            update_terminos();
        }
        line = readline("massiveshell$ ");
    }
    printf("byeEeEeEe...\n");
    // TODO: free memory before exiting
    // This function should also be called when exiting the shell using `exit` builtin
    reset_terminos();
    return (0);
}

int main(int argc, char **argv, char **envp)
{
	char **envp_copy;

    (void)argv;
    if (argc >= 2)
        return (1); // TODO: handle non-interactive mode if required or desired
    if (isatty(STDIN_FILENO))
    {
		envp_copy = copy_env(envp);
        use_parent_signals();
        // TODO: Start an interactive shell and do magic
        interactive_mode(envp_copy);
    }
    return (g_exit_code);
}

