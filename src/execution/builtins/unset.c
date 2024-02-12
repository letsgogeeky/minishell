/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:55:17 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/12 00:57:26 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/builtins.h"
#include "minishell/error.h"
#include "minishell/minishell.h"

int	builtins_unset(char **args, t_minishell *ms)
{
	int	i;

	if (!ms->envp)
		return (print_builtin_error("unset", NULL, "no envp"), EXIT_FAILURE);
	i = 0;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
			return (print_builtin_error(
					"unset", args[i], "not a valid identifier"), \
				EXIT_FAILURE);
		if (exists_in_env(args[i], ms->envp) != -1)
			ms->envp = remove_from_env(args[i], ms->envp);
		i++;
	}
	return (EXIT_SUCCESS);
}
