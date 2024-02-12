/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_sanitizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:29:52 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/12 00:31:22 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

char	*trim_start(char *str, bool free_str)
{
	char	*trimmed;
	int		i;
	int		j;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	trimmed = malloc(ft_strlen(str) - i + 1);
	if (!trimmed)
		return (NULL);
	j = 0;
	while (str[i])
	{
		trimmed[j] = str[i];
		i++;
		j++;
	}
	trimmed[j] = '\0';
	if (free_str)
		free(str);
	return (trimmed);
}

char	*trim_end(char *str, bool free_str)
{
	char	*trimmed;
	int		i;
	int		j;

	i = 0;
	while (str[i])
		i++;
	i--;
	while (str[i] == ' ' || str[i] == '\t')
		i--;
	trimmed = malloc(i + 2);
	if (!trimmed)
		return (NULL);
	j = 0;
	while (j <= i)
	{
		trimmed[j] = str[j];
		j++;
	}
	trimmed[j] = '\0';
	if (free_str)
		free(str);
	return (trimmed);
}
