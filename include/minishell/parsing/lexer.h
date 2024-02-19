/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 01:25:36 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 01:18:35 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <stdlib.h>
# include <stdio.h>

typedef enum e_token_type
{
	WORD ,
	ASSIGNMENT,
	OPTION,
	DLESS,
	DGREAT,
	LESS,
	GREAT,
	PIPE,
	EOF_TOKEN ,
	ERROR_TOKEN
}	t_token_type;

typedef enum e_state
{
	start,
	in_word,
	in_option,
	in_separator,
	in_dless,
	in_dgreat,
	in_less,
	in_great,
	in_pipe,
	in_assignment,
	in_dqoute,
	in_squote,
	in_escape,
	in_error
}	t_state;

typedef struct s_token
{
	t_token_type	type;
	char			*lexeme;
}	t_token;

typedef struct s_lexer
{
	char	*input_buffer;
	char	*current;
	char	*start;
	t_state	state;
}	t_lexer;

t_token	*new_token(t_token_type type, t_lexer *lexer);
t_token	*init_token_array(size_t capacity);
t_token	*realloc_token_array(t_token *tokens, size_t *capacity);
void	destroy_tokens(t_token *tokens);

t_token	*pipe_token(t_lexer *lexer);

t_token	*lex(const char *input);
t_token	*redirect_token(t_lexer *lexer);
t_token	*dredirect_token(t_lexer *lexer);
t_token	*word_token(t_lexer *lexer);
t_token	*option_token(t_lexer *lexer);

t_token	*assignment_token(t_lexer *lexer);
t_token	*quoted_token(t_lexer *lexer);
t_token	*escaped_token(t_lexer *lexer);
t_token	*command_token(t_lexer *lexer);
t_token	*error_token(t_lexer *lexer);

void	log_tokens(t_token *tokens);
#endif