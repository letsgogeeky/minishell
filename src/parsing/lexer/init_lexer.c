/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 02:54:06 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/14 04:05:38 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

#include "minishell/error.h"
#include "minishell/parsing/lexer.h"


void	init_lexer(t_lexer *lexer, char *input)
{
	lexer->state = START;
	lexer->input = input;
	lexer->token = NULL;
	lexer->position = 0;
}
t_token	**create_token_array(int init_size)
{
	t_token	*array;

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
	if (!new_token)
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