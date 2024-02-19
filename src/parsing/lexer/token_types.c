/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 00:30:29 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 00:31:49 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/lexer.h"
#include "baselib.h"

t_token	*pipe_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	++lexer->current;
	return (new_token(PIPE, lexer));
}

t_token	*redirect_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	if (*(lexer->current) == '<')
		return (++lexer->current, new_token(LESS, lexer));
	else if (*(lexer->current) == '>')
		return (++lexer->current, new_token(GREAT, lexer));
	return (new_token(ERROR_TOKEN, lexer));
}

t_token	*dredirect_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	if (*(lexer->current) == '<' && *(lexer->current + 1) == '<')
		return (lexer->current += 2, new_token(DLESS, lexer));
	else if (*(lexer->current) == '>' && *(lexer->current + 1) == '>')
		return (lexer->current += 2, new_token(DGREAT, lexer));
	return (new_token(ERROR_TOKEN, lexer));
}

t_token	*word_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	while (*lexer->current != '\0' && !ra_is_whiteshapce(*lexer->current) \
		&& *lexer->current != '|' && *lexer->current != '<' \
		&& *lexer->current != '>' && *lexer->current != '=')
	{
		if (*lexer->current == '\\')
		{
			++lexer->current;
			if (*lexer->current == '\0')
				++lexer->current;
		}
		else
			++lexer->current;
	}
	return (new_token(WORD, lexer));
}

t_token	*option_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	if (*(lexer->current + 1) == '-')
		lexer->current += 2;
	else
		++lexer->current;
	while (*lexer->current != '\0' && !ra_is_whiteshapce(*lexer->current) \
		&& *lexer->current != '|' && *lexer->current != '<' \
		&& *lexer->current != '>' && *lexer->current != '=')
		++lexer->current;
	return (new_token(OPTION, lexer));
}
