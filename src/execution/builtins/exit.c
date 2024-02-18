/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:48:32 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 01:56:07 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/error.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"

void	builtins_exit(t_minishell *ms, char **options, char *cmd)
{
	if (options && options[0])
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		ms->exit_code = 1;
		return ;
	}
	printf("exit\n");
	clear_history();
	reset_terminos();
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
	free(ms);
	free(cmd);
	exit(0);
}
