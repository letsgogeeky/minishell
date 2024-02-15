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
    ms->input = readline("massiveshell$ ");
    while (ms->input && ms->input[0] != EOF)
    {
		if (ms->input[0] != '\0' && !is_empty(ms->input))
		{
			printf("input: %s\n", ms->input);
			ms->ast = parse_input(ms->input);
			printf("\nast: %p\n", ms->ast);
			print_ast(ms->ast, 0);
			add_history(ms->input);
            reset_terminos();
			expand_ast(ms, ms->ast, 0);
			ms->count = count_cmds(ms, ms->ast, false);
			init_fds(ms);
			execute_ast(ms, ms->ast);
			post_execute_destroy(ms);
            update_terminos();
        }
        ms->input = readline("massiveshell$ ");
		use_parent_signals();
    }
    printf("byeEeEeEe...\n");
	destroy_ms(ms);
	printf("Destroyed ms\n");
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
		ms->exit_code = 0;
		ms->file_node = NULL;
		ms->stack = NULL;
		ms->first_cmd = NULL;
		ms->envp = get_environment();
        use_parent_signals();
        interactive_mode(ms);
    }
    return (0);
}
