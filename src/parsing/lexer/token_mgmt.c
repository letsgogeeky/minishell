/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_mgmt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 00:25:46 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 00:29:04 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/lexer.h"
#include "baselib.h"

t_token	*new_token(t_token_type type, t_lexer *lexer)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		ft_putstr_fd("Failed to allocate memory for token\n", STDERR_FILENO);
		exit(1);
	}
	token->type = type;
	token->lexeme = ft_substr(lexer->start, 0, lexer->current - lexer->start);
	return (token);
}

t_token	*init_token_array(size_t capacity)
{
	t_token	*tokens;

	tokens = malloc(capacity * sizeof(t_token));
	if (!tokens)
	{
		ft_putstr_fd("Failed to allocate memory for tokens\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (tokens);
}

t_token	*realloc_token_array(t_token *tokens, size_t *capacity)
{
	t_token	*temp;

	*capacity *= 2;
	temp = malloc(*capacity * sizeof(t_token));
	if (!temp)
	{
		ft_putstr_fd("Failed to reallocate memory for tokens\n", STDERR_FILENO);
		destroy_tokens(tokens);
		exit(EXIT_FAILURE);
	}
	ft_memcpy(temp, tokens, *capacity / 2 * sizeof(t_token));
	free(tokens);
	return (temp);
}

void	destroy_tokens(t_token *tokens)
{
	size_t	i;

	i = -1;
	while (tokens[++i].type != EOF_TOKEN)
		free(tokens[i].lexeme);
	free(tokens[i].lexeme);
	free(tokens);
}
