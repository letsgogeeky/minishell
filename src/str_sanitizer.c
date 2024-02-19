/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_sanitizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:29:52 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/19 03:45:39 by fvoicu           ###   ########.fr       */
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

char	*get_dequoted_value(char *value)
{
	if (!value || ft_strlen(value) < 2)
		return (ft_strdup(value));
	if (value[0] != '\'' && value[0] != '\"')
		return (ft_strdup(value));
	if ((value[0] == '\'' && value[ft_strlen(value) - 1] == '\'') || \
		(value[0] == '\"' && value[ft_strlen(value) - 1] == '\"'))
		return (ft_substr(value, 1, ft_strlen(value) - 2));
	return (ft_strdup(value));
}
