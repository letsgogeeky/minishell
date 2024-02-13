#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/execution/executor.h"
#include "minishell/parsing/parser.h"

struct termios	settings;
static	bool is_empty(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

static int interactive_mode(t_minishell *ms)
{
    // int     out_file_fd;
    // int     in_fd;
    // bool    is_append;

    ms->input = readline("massiveshell$ ");
    while (ms->input && ms->input[0] != EOF)
    {
		if (ms->input[0] != '\0' && !is_empty(ms->input))
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
			printf("input: %s\n", ms->input);
			ms->ast = parse_input(ms->input);
			print_ast(ms->ast, 0);
            ms->cmds = ft_split(ms->input, '|');
			int x = 0;
			while (ms->cmds[x])
			{
				ms->cmds[x] = trim_start(trim_end(ms->cmds[x], true), true);
				x++;
			}
			add_history(ms->input);
            reset_terminos();
			iterate_ast(ms, ms->ast, 0);
            // expand(ms);
			execute_ast(ms, ms->ast);
			// executor(ms, 0); // TODO: make this proper executor
            update_terminos();
			str_arr_free(ms->cmds);
        }
        ms->input = readline("massiveshell$ ");
		use_parent_signals();
    }
    printf("byeEeEeEe...\n");
    // TODO: free memory before exiting
    // This function should also be called when exiting the shell using `exit` builtin
    reset_terminos();
    return (0);
}

int main(int argc, char **argv)
{
	t_minishell *ms;

	ms = (t_minishell *)malloc(sizeof(t_minishell));
    if (argc >= 2)
        return (1); // TODO: handle non-interactive mode if required or desired
    if (isatty(STDIN_FILENO))
    {
		ms->args = argv;
		ms->in_fd = -1;
		ms->out_fd = -1;
		ms->exit_code = 0;
		ms->envp = get_environment();
        use_parent_signals();
        interactive_mode(ms);
    }
    return (ms->exit_code);
}
