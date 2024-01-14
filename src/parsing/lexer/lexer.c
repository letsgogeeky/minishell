/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 02:54:09 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/14 04:02:12 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/error.h"
#include "minishell/parsing/lexer.h"

t_type	get_token_type(char current, char next)
{
	if (current == '<' && next == '<')
		return (DLESS);
	else if(current == '>' && next == '>')
		return (DGREAT);
	if (current == '|')
		return (PIPE);
	else if (current == '<')
		return (LESS);
	else if (current == '>')
		return (GREAT);
	else if (current == '=')
		return (ASSIGN);
	else if (current == '\0')
		return (EOF);
	return (WORD);
}

t_state get_state(int prev_state, int token)
{
	static int state_table[STATE_NB * TOKEN_NB] = {
		READ_CMD, ERROR, READ_INPUT, READ_OUTPUT, END_CMD, ERROR, \
		READ_CMD, READ_PIPE, READ_INPUT, READ_OUTPUT,END_CMD, ERROR, \
		READ_CMD, ERROR, ERROR, ERROR, END_CMD, ERROR, \
		READ_CMD, READ_PIPE, READ_HEREDOC, READ_CMD, END_CMD, ERROR, \
		READ_CMD, READ_PIPE, READ_INPUT, READ_CMD, END_CMD, ERROR, \
		READ_CMD, READ_PIPE, READ_HEREDOC, READ_CMD, END_CMD, ERROR, \
		READ_CMD, READ_PIPE, READ_INPUT, READ_CMD, END_CMD, ERROR, \
		ERROR, ERROR, ERROR, ERROR, END_CMD, ERROR, \
		ERROR, ERROR, ERROR, ERROR, ERROR, ERROR
	};
	return (state_table[prev_state * TOKEN_NB + token]);
}

void	tokenize(t_lexer *lexer)
{
	t_token *new_token;
	t_type	token_type;
	t_state state;
	char	current;
	char	next;

	while (lexer->input[lexer->position])
	{
		current = lexer->input[lexer->position];
		next = lexer->input[lexer->position + 1];
		token_type = get_token_type(current, next);
		state = get_state(lexer->state, token_type);
		lexer->state = state;
		if (state == ERROR)
			error("syntax error near unexpected token");
		// if (/*condition for token completion*/)
		// 	new_token = create_token(token_type, /*token value*/);
		// if (state == ERROR)
		// 	// error
		// else if (state == END_CMD)
		// 	// add token to list
		
		if (token_type == DLESS || token_type == DGREAT)
			++lexer->position;
		
		++lexer->position;
	}
}
