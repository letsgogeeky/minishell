/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:38:37 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/12 00:54:47 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/error.h"
#include "minishell/execution/builtins.h"

int	builtins_pwd(void)
{
	char	path[PATH_MAX];

	if (!getcwd(path, PATH_MAX))
	{
		print_builtin_error("pwd", NULL, strerror(errno));
		return (EXIT_FAILURE);
	}
	printf("%s\n", path);
	return (EXIT_SUCCESS);
}
