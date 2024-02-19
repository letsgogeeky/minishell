/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 21:14:22 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 02:23:25 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/execution/executor.h"
#include "minishell/parsing/parser.h"

static bool	is_empty(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] != ' ' && str[i] != '\t')
			return (false);
	return (true);
}

static void	deploy(t_minishell *ms)
{
	ms->ast = parse_input(ms->input);
	add_history(ms->input);
	reset_terminos();
	expand_ast(ms, ms->ast, 0);
	ms->count = count_cmds(ms, ms->ast, false);
	init_fds(ms);
	execute_ast(ms, ms->ast);
	post_execute_destroy(ms);
	update_terminos();
}

static int	interactive_mode(t_minishell *ms)
{
	char	*rl_input;
	int	i;
	
	i = -1;
	rl_input = readline("massiveshell$ ");
	ms->rl_input = ft_split(rl_input, '\n');
	while (rl_input && rl_input[0] != EOF)
	{
		free(rl_input);
		while (ms->rl_input && ms->rl_input[++i] && ms->rl_input[i][0] != EOF)
		{
			ms->input = ft_strdup(ms->rl_input[i]);
			if (ms->input[0] != '\0' && !is_empty(ms->input))
				deploy(ms);
			use_parent_signals();
		}
		if (ms->rl_input[i] && ms->rl_input[i][0] != EOF)
			break ;
		str_arr_free(ms->rl_input);
		rl_input = readline("massiveshell$ ");
		ms->rl_input = ft_split(rl_input, '\n');
		i = -1;
	}
	return (destroy_ms(ms), 0);
}

char	*join_args(char **args)
{
	int		i;
	char	*str;
	char	*tmp;

	i = 1;
	str = ft_strdup(args[i]);
	while (args[++i])
	{
		tmp = ft_strjoin(str, " ");
		free(str);
		str = ft_strjoin(tmp, args[i]);
		free(tmp);
	}
	return (str);
}

int	main(int argc, char **argv)
{
	t_minishell	*ms;
	int			exit_code;

	ms = (t_minishell *)malloc(sizeof(t_minishell));
	ms->args = argv;
	ms->exit_code = 0;
	ms->file_node = NULL;
	ms->stack = NULL;
	ms->first_cmd = NULL;
	ms->envp = get_environment();
	use_parent_signals();
	if (argc == 1 && isatty(STDIN_FILENO))
		return (interactive_mode(ms));
	ms->input = join_args(argv);
	deploy(ms);
	exit_code = ms->exit_code;
	free(ms);
	return (exit_code);
}
