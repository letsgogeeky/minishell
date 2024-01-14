/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 01:25:36 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/14 03:55:40 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# define STATE_NB 6
# define TOKEN_NB 9

typedef enum e_state
{
	START,
	READ_CMD,
	READ_PIPE,
	READ_REDIR, 
	READ_INPUT,
	READ_OUTPUT,
	// READ_APPEND,
	READ_HEREDOC,
	END_CMD,
	ERROR,
}	t_state;

typedef enum e_input_type
{
	WORD,
	ASSIGN,
	PIPE,
	DLESS,
	DGREAT,
	LESS,
	GREAT,
	EOF,
	OTHER
}	t_type;

typedef struct s_token
{
	t_type	type;
	char	*value;
}	t_token;

typedef struct s_lexer
{
	t_state		state;
	char		*input;
	t_token		*token;
	int			position;

}	t_lexer;




#endif