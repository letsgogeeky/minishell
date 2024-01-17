/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 02:54:09 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/17 15:18:09 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// #include "minishell/error.h"
// #include "minishell/parsing/lexer.h"

/*FOR TEST*/
# define STATE_NB 9
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
	ENOF,
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

///*FOR TEST*/


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
		return (ENOF);
	return (WORD);
}
//TODO: complete state table
t_state get_state(int prev_state, int token)
{
	static int state_table[STATE_NB * TOKEN_NB] = {
		READ_CMD, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, \
		READ_CMD, READ_CMD, READ_PIPE, READ_INPUT, READ_OUTPUT, READ_HEREDOC, READ_APPEND, END_CMD, ERROR, \
		ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, \
		ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, \
		ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, \
		ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, \
		ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, \
		ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, \
		ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, \
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
			printf("syntax error near unexpected token");
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

// Test function
bool test_state_transition(int prev_state, int token, t_state expected_state) {
    t_state actual_state = get_state(prev_state, token);
    if (actual_state != expected_state) {
        printf("Test failed: Prev State: %d, Token: %d, Expected: %d, Actual: %d\n", prev_state, token, expected_state, actual_state);
        return false;
    }
    return true;
}

int main() {
    bool all_tests_passed = true;

    // Add your test cases here
    // all_tests_passed &= test_state_transition(START, WORD, READ_CMD);
	// all_tests_passed &= test_state_transition(START, PIPE, ERROR);
	all_tests_passed &= test_state_transition(START, WORD, READ_CMD);
	all_tests_passed &= test_state_transition(START, ASSIGN, ERROR);
	all_tests_passed &= test_state_transition(START, PIPE, READ_INPUT);
	// all_tests_passed &= test_state_transition(READ_INPUT, PIPE, READ_PIPE);
    // More tests...

    if (all_tests_passed) {
        printf("All tests passed!\n");
    } else {
        printf("Some tests failed.\n");
    }

    return all_tests_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
