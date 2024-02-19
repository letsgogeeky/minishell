/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 00:52:19 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 04:07:26 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/lexer.h"
#include "baselib.h"

void	print_token(const t_token *token)
{
	if (token->type == WORD)
		printf("WORD(%s) ", token->lexeme);
	else if (token->type == ASSIGNMENT)
		printf("ASSIGNMENT(%s) ", token->lexeme);
	else if (token->type == DLESS)
		printf("DLESS ");
	else if (token->type == DGREAT)
		printf("DGREAT ");
	else if (token->type == LESS)
		printf("LESS ");
	else if (token->type == GREAT)
		printf("GREAT ");
	else if (token->type == PIPE)
		printf("PIPE ");
	else if (token->type == OPTION)
		printf("OPTION(%s) ", token->lexeme);
	else if (token->type == EOF_TOKEN)
		printf("EOF_TOKEN ");
	else if (token->type == ERROR_TOKEN)
		printf("ERROR_TOKEN ");
	else
		printf("UNKNOWN ");
}

void	log_tokens(t_token *tokens)
{
	int	i;

	i = -1;
	while (tokens[++i].type != EOF_TOKEN)
	{
		printf("-->");
		print_token(&tokens[i]);
	}
	print_token(&tokens[i]);
	printf("\n");
}
