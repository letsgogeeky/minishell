/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:43:22 by ramoussa          #+#    #+#             */
/*   Updated: 2023/12/27 18:35:26 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"

// TODO: change arguments to be the expected structs
int	exec_cmd(char *cmd, char **envp)
{
	// TODO: handle when path is NULL
	printf("%s\n", get_path(cmd, envp));
	return (EXIT_SUCCESS);
}
