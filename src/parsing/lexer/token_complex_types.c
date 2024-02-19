/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_complex_types.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 00:32:09 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 00:32:55 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/lexer.h"
#include "baselib.h"

t_token	*assignment_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	++lexer->current;
	return (new_token(ASSIGNMENT, lexer));
}

t_token	*quoted_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	lexer->current++;
	while (*lexer->current && *lexer->current != *lexer->start)
	{
		lexer->current++;
		if (*lexer->current == '\\')
			lexer->current += 2;
	}
	if (*lexer->current == *lexer->start)
		lexer->current++;
	return (new_token(WORD, lexer));
}

t_token	*escaped_token(t_lexer *lexer)
{
	lexer->start = ++lexer->current;
	++lexer->current;
	return (new_token(WORD, lexer));
}

t_token	*command_token(t_lexer *lexer)
{
	if (lexer->state == in_word)
		return (word_token(lexer));
	else
		return (option_token(lexer));
}

t_token	*error_token(t_lexer *lexer)
{
	++lexer->current;
	return (new_token(ERROR_TOKEN, lexer));
}
