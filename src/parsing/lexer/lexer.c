/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 12:19:20 by fvoicu            #+#    #+#             */
/*   Updated: 2023/12/28 03:04:58 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/error.h"
#include "minishell/parsing/lexer.h"


//1.Read the input
//2.Tokenize the input
//3.Store it in a token array 
//4. Error handling

void	set_token(t_token *this_token)
{
	char	*token;

	token = this_token->value;
 	if (strchr(token, '|') && !strchr(token, '<') && !strchr(token, '>'))
		return (this_token->type = PIPE);
	if (!strchr(token, '|') && strchr(token, '<') && !strchr(token, '>'))
		return (this_token->type = INPUT);
	if (!strchr(token, '|') && !strchr(token, '<') && strchr(token, '>')
		&& strlen(token) == 1)
		return (this_token->type = OUTPUT);
	if (!strchr(token, '|') && strchr(token, '<') && !strchr(token, '>')
		&& strlen(token) == 2)
		return (this_token->type = HEREDOC);
	if (!strchr(token, '|') && !strchr(token, '<') && strchr(token, '>')
		&& strlen(token) == 2)
		return (this_token->type = APPEND);
	this_token->type = SYNTAX;
}

//State of the lexer based on the cuurent state and the input character
int	get_state(int state, char input)
{
	//Get the next state based on the current state and the input character
	//Transition table
}

int lexer(char *input)
{
	//Split the input into tokens
	//Categorize the tokens by their type, value and position
	//And fill the token stucture
}

