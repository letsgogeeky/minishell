/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:43:22 by ramoussa          #+#    #+#             */
/*   Updated: 2023/12/26 19:59:58 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"

int	exec_cmd(char *cmd, char **envp)
{
	printf("%s\n", get_path(cmd, envp));
	return (0);
}
