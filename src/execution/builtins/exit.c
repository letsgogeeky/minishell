/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:48:32 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/12 00:48:49 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/error.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"

void	builtins_exit(t_minishell *ms)
{
	clear_history();
	reset_terminos();
	str_arr_free(ms->cmds);
	free_env(ms->envp);
	exit(ms->exit_code);
}
