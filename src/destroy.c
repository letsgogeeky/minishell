/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 01:43:44 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 02:00:53 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

void	destroy_ms(t_minishell *ms)
{
	clear_history();
	reset_terminos();
	str_arr_free(ms->envp);
	if (ms->first_cmd)
		free(ms->first_cmd);
	free(ms);
}

void	post_execute_destroy(t_minishell *ms)
{
	if (ms->input)
		free(ms->input);
	ms->input = NULL;
	if (ms->ast)
	{
		if (LOG_DETAILS)
			printf("\033[1;31m Offloading AST:::: \033[0m \n");
		destroy_ast(ms->ast);
	}
	if (ms->first_cmd)
		free(ms->first_cmd);
	ms->first_cmd = NULL;
	ms->ast = NULL;
	ms->file_node = NULL;
}
