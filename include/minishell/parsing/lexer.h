/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 01:25:36 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/03 01:35:42 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #ifndef LEXER_H
// # define LEXER_H

// typedef enum e_state
// {
// 	START,
// 	READ_CMD,
// 	READ_PIPE,
// 	READ_INPUT,
// 	READ_OUTPUT,
// 	READ_APPEND,
// 	READ_HEREDOC,
// 	END_CMD,
// 	ERROR,
// }	t_state;

// typedef enum e_input_type
// {
// 	WORD,
// 	ASSIGN,
// 	PIPE,
// 	LESS,
// 	GREAT,
// 	DLESS,
// 	DGREAT,
// 	EOF,
// 	OTHER
// }	t_type;

// typedef struct s_token
// {
// 	t_type	type;
// 	char	*value;
// }	t_token;

// typedef struct s_lexer
// {
// 	t_state		state;
// 	char		*input;
// 	t_token		*token;
// 	int			position;

// }	t_lexer;




// #endif

// #ifndef LEXER_H
// #define LEXER_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// typedef enum {
//   WORD ,
//   ASSIGNMENT_WORD,
//   DLESS,
//   DGREAT,
//   LESS,
//   GREAT,
//   PIPE,
//   EOF_TOKEN ,
//   ERROR_TOKEN
// } TokenType;

// typedef enum e_state{

//   START = 9,
//   IN_WORD,
//   IN_DLESS,
//   IN_DGREAT,
//   IN_LESS,
//   IN_GREAT,
//   IN_PIPE,
//   IN_ASSIGNMENT_WORD
// }	t_state;

// typedef struct Token {
//   TokenType type;
//   char *lexeme;
//   int line;
// } Token;

// typedef struct Lexer {
//   char *input_buffer;
//   char *current;
//   char *start;
//   int line_number;
//   t_state state;
// } Lexer;

// void lexer_init(Lexer *lexer, const char *filename);
// void lexer_destroy(Lexer *lexer);
// Token *get_next_token(Lexer *lexer);
// Token *new_token(int type, Lexer *lexer);
// Token *new_error_token(Lexer *lexer);

// #endif