/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:31:42 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/12 00:33:24 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

int	error_file_not_found(char *path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	return (-1);
}

int	error_permission_denied(char *path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	return (-1);
}

/*
>> O_APPEND
> O_TRUNC
< O_RDONLY
*/
int	open_file(char *path, int flags)
{
	int	fd;

	if (flags == O_RDONLY && access(path, F_OK) == -1)
		return (error_file_not_found(path));
	if (flags == O_RDONLY && access(path, R_OK) == -1)
		return (error_permission_denied(path));
	if (flags > O_RDONLY && !access(path, F_OK) && access(path, W_OK) == -1)
		return (error_permission_denied(path));
	if (flags == O_RDONLY)
		fd = open(path, flags);
	else
		fd = open(path, O_WRONLY | O_CREAT | flags, 0644);
	return (fd);
}
