/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:31:42 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 21:14:05 by fvoicu           ###   ########.fr       */
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

int	open_file(char *path, int flags)
{
	int	fd;
	int	access_ret;

	access_ret = access(path, F_OK);
	if (flags == O_RDONLY && access_ret == -1)
		return (error_file_not_found(path));
	if (flags == O_RDONLY && access(path, R_OK) == -1)
		return (error_permission_denied(path));
	if (flags > O_RDONLY)
	{
		fd = open(path, O_WRONLY | O_CREAT | flags, 0644);
		if (fd == -1)
			return (error_permission_denied(path));
	}
	if (flags == O_RDONLY)
		fd = open(path, flags);
	return (fd);
}
