/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 01:25:36 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/12 02:09:20 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #ifndef LEXER_H
// # define LEXER_H
// typedef enum e_token_type
// {
// 	WORD ,
// 	ASSIGNMENT,
// 	OPTION,
// 	DLESS,
// 	DGREAT,
// 	LESS,
// 	GREAT,
// 	PIPE,
// 	EOF_TOKEN ,
// 	ERROR_TOKEN
// }	t_token_type;

// typedef enum e_state
// {
// 	start,
// 	in_word,
// 	in_option,
// 	in_separator,
// 	in_dless,
// 	in_dgreat,
// 	in_less,
// 	in_great,
// 	in_pipe,
// 	in_assignment,
// 	in_dqoute,
// 	in_squote,
// 	in_escape,
// 	in_error
// }	t_state;

// typedef struct s_token
// {
// 	t_token_type	type;
// 	char			*lexeme;
// }	t_token;

// typedef struct s_lexer
// {
// 	char	*input_buffer;
// 	char	*current;
// 	char	*start;
// 	t_state	state;
// }	t_lexer;
// #endif