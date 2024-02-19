/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:48:32 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/19 03:11:23 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/error.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"

void	free_cmd_related(t_minishell *ms);

int	builtins_exit(t_minishell *ms, char **options, char *cmd)
{
	int	exit_code;

	exit_code = ms->exit_code;
	if (options && options[0])
	{
		if (options[0][0] != '-' && !ra_is_int(options[0]))
		{
			print_builtin_error("exit", options[0], \
				"numeric argument required");
			exit_code = 255;
		}
		else if (options[0][0] != '-' && ra_is_int(options[0]))
			exit_code = ft_atoi(options[0]);
		else
		{
			ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
			ms->exit_code = 1;
			return (EXIT_FAILURE);
		}
	}
	printf("exit\n");
	clear_history();
	reset_terminos();
	free_cmd_related(ms);
	return (free(cmd), free(ms), exit(exit_code), EXIT_SUCCESS);
}

void	free_cmd_related(t_minishell *ms)
{
	str_arr_free(ms->envp);
	if (ms->first_cmd)
		free(ms->first_cmd);
	if (ms->input)
		free(ms->input);
	if (ms->ast)
	{
		if (LOG_DETAILS)
			printf("\033[1;31m Offloading AST:::: \033[0m \n");
		destroy_ast(ms->ast);
	}
	str_arr_free(ms->rl_input);
}
