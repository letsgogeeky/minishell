/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_mgmt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 19:27:59 by fvoicu            #+#    #+#             */
/*   Updated: 2023/12/28 02:39:32 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
//TODO: Remove and use a libft function
#include <string.h>
#include "minishell/error.h"
#include "minishell/parsing/lexer.h"

t_token	**create_token_array(int init_size)
{
	t_token	array;

	array = (t_token **)malloc(init_size * sizeof(t_token *));
	if (!array)
		//error
		return (NULL);
	return (array);
}
t_token *create_token(t_type type, const char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!token)
		//error
		return (NULL);
	new_token->type = type;
	new_token->value = strdup(value);
	return (new_token);
}

bool	add_token(t_token ***array, t_token *token, int *size, int *capacity)
{
	t_token **temp;
	if (*size >= *capacity)
	{
		*capacity *= 2;
		temp = realloc_token_array(*array, *capacity * sizeof(t_token *));
		//TODO: relloc failed case should throw a specific error?
		if (!temp)
			return (false);
		*array = temp;
	}
	(*array)[*size] = token;
	(*size)++;
	return (true);
}
