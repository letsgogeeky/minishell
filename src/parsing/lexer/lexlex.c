// #include "minishell/parsing/lexer.h"
// TODO: separate the funcs
// TODO: ft_putstr_fd -->for error handling
// TODO: do ft_realloc, ft_isspace
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "minishell/parsing/lexer.h"
#include "baselib.h"



void	print_token(const t_token* token);
void	log_tokens(t_token *tokens);

void	init_lexer(t_lexer *lexer, const char *input)
{
	lexer->input_buffer = strdup(input);
	if (!lexer->input_buffer)
	{
		ft_putstr_fd("Failed to allocate memory for input buffer\n", STDERR_FILENO);
		exit(1);
	}
	lexer->current = lexer->input_buffer;
	lexer->start = lexer->input_buffer;
	lexer->state = start;
}

void	destroy_lexer(t_lexer *lexer)
{
	free(lexer->input_buffer);
}

t_token *new_token(t_token_type type, t_lexer *lexer)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
	{
		ft_putstr_fd("Failed to allocate memory for token\n", STDERR_FILENO);
		exit(1);
	}
	token->type = type;
	token->lexeme = ft_substr(lexer->start, 0, lexer->current - lexer->start);
	return (token);
}

void	update_state(t_lexer *lexer, char curr_ch)
{
	lexer->state = in_error;
	if (ra_is_whiteshapce(curr_ch))
		lexer->state = in_separator;
	else if (curr_ch == '|')
		lexer->state = in_pipe;
	else if (curr_ch == '<' && *(lexer->current + 1) != '<')
		lexer->state = in_less;
	else if (curr_ch == '>' && *(lexer->current + 1) != '>')
		lexer->state = in_great;
	else if (curr_ch == '<' && *(lexer->current + 1) == '<')
		lexer->state = in_dless;
	else if (curr_ch == '>' && *(lexer->current + 1) == '>')
		lexer->state = in_dgreat;
	else if (curr_ch == '=')
		lexer->state = in_assignment;
	else if (curr_ch == '-')
		lexer->state = in_word;
	else if (curr_ch == '"')
		lexer->state = in_dqoute;
	else if (curr_ch == '\'')
		lexer->state = in_squote;
	// else if (curr_ch == '\\')
	// 	lexer->state = in_escape;
	else if (curr_ch != '\0')
		lexer->state = in_word;
}

//Parse 
t_token	*pipe_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	++lexer->current;
	return (new_token(PIPE, lexer));
}

t_token	*redirect_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	if(*(lexer->current) == '<')
		return (++lexer->current, new_token(LESS, lexer));
	else if(*(lexer->current) == '>')
		return (++lexer->current, new_token(GREAT, lexer));
	return(new_token(ERROR_TOKEN, lexer));
}

t_token	*dredirect_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	if(*(lexer->current) == '<' && *(lexer->current + 1) == '<')
		return (lexer->current += 2, new_token(DLESS, lexer));
	else if(*(lexer->current) == '>' && *(lexer->current + 1) == '>')
		return (lexer->current += 2, new_token(DGREAT, lexer));
	return(new_token(ERROR_TOKEN, lexer));
}
t_token	*word_token(t_lexer *lexer)
{
	lexer->start = lexer->current;
	while (*lexer->current != '\0' && !isspace(*lexer->current) \
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
	return(new_token(WORD, lexer));
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
	return(new_token(OPTION, lexer));
}

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
t_token *command_token(t_lexer *lexer)
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


t_token	*get_next_token(t_lexer *lexer)
{
	while (*lexer->current)
	{
		update_state(lexer, *lexer->current);
		if (lexer->state == in_word || lexer->state == in_option)
			return(command_token(lexer));
		else if (lexer->state == in_pipe)
			return(pipe_token(lexer));
		else if (lexer->state == in_less || lexer->state == in_great)
			return(redirect_token (lexer));
		else if (lexer->state == in_dless || lexer->state == in_dgreat)
			return(dredirect_token (lexer));
		else if (lexer->state == in_assignment)
			return(assignment_token(lexer));
		else if(lexer->state == in_dqoute || lexer->state == in_squote)
			return(quoted_token(lexer));
		// else if (lexer->state == in_escape)
		// 	return(escaped_token(lexer));
		else if (lexer->state == in_separator)
			++lexer->current;
		else if (lexer->state == in_error)
			return(error_token(lexer));
	}
	return (new_token(EOF_TOKEN, lexer));
}

t_token	*init_token_array(size_t capacity)
{
	t_token *tokens;

	tokens = malloc(capacity * sizeof(t_token));
	if(!tokens)
	{
		ft_putstr_fd("Failed to allocate memory for tokens\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (tokens);
}

void	destroy_tokens(t_token *tokens)
{
	size_t i;

	i = 0;
	while (tokens[i].type != EOF_TOKEN)
	{
		free(tokens[i].lexeme);
		i++;
	}
	free(tokens[i].lexeme);
	free(tokens);
}

t_token	*realloc_token_array(t_token *tokens, size_t *capacity)
{
	*capacity *= 2;
	t_token *temp = malloc(*capacity * sizeof(t_token));
	if (!temp)
	{
		ft_putstr_fd("Failed to reallocate memory for tokens\n", STDERR_FILENO);
		destroy_tokens(tokens);
		exit(EXIT_FAILURE);
	}
	ft_memcpy(temp, tokens, *capacity / 2 * sizeof(t_token));
	free(tokens);
	return (temp);
}

t_token *lex(const char *input)
{
	t_lexer lexer;
	size_t token_capacity;
	t_token *tokens;
	size_t tokens_size;
	t_token	*tmp;

	token_capacity = 10;
	tokens = init_token_array(token_capacity);
	tokens_size = 0;
	init_lexer(&lexer, input);
	while (1)
	{
		if (tokens_size >= token_capacity)
			tokens = realloc_token_array(tokens, &token_capacity);
		tmp = get_next_token(&lexer);
		tokens[tokens_size] = *tmp;
		free(tmp);
		if (tokens[tokens_size].type == EOF_TOKEN) {
			break;
		}
		++tokens_size;
	}
	destroy_lexer(&lexer);
	return (tokens);
}

//<TESTS>//

/*************************************************************************************/
/************************************ TESTS ******************************************/
/*************************************************************************************/

void print_token(const t_token* token) {
    switch (token->type) {
        case WORD: printf("WORD(%s) ", token->lexeme); break;
        case ASSIGNMENT: printf("ASSIGNMENT(%s) ", token->lexeme); break;
        case DLESS: printf("DLESS "); break;
        case DGREAT: printf("DGREAT "); break;
        case LESS: printf("LESS "); break;
        case GREAT: printf("GREAT "); break;
        case PIPE: printf("PIPE "); break;
		case OPTION: printf("OPTION(%s) ", token->lexeme); break;
        case EOF_TOKEN: printf("EOF_TOKEN "); break;
        case ERROR_TOKEN: printf("ERROR_TOKEN "); break;
        default: printf("UNKNOWN ");
    }
}

void	log_tokens(t_token *tokens)
{
	size_t i = 0;

    while (tokens[i].type != EOF_TOKEN) {
        printf("-->");
        print_token(&tokens[i]);
        i++;
    }
	print_token(&tokens[i]);
	printf("\n");
}

/*
For testing tokens separately
// */
// void run_test(const char *test_input) {
//     t_lexer lexer;
//     init_lexer(&lexer, test_input);

//     printf("Test Input: \"%s\"\n", test_input);
//     printf("Tokens: ");
//     t_token *token;
//     while ((token = get_next_token(&lexer))->type != EOF_TOKEN) {
// 		printf("-->");
//         print_token(token);
//         free(token);
//     }
// 	printf("-->");
//     print_token(token); // Print EOF_TOKEN
//     free(token);

//     destroy_lexer(&lexer);
//     printf("\n");
// }

/*
For lex:
*/
void run_test(const char *test_input) {
    printf("Test Input: \"%s\"\n", test_input);
    printf("Tokens: ");

    // Call lex to tokenize the entire input
    t_token *tokens = lex(test_input);

    // Iterate over the tokens array until you hit the EOF_TOKEN
    size_t i = 0;
    while (tokens[i].type != EOF_TOKEN) {
        printf("-->");
        print_token(&tokens[i]);
        i++;
    }
    printf("-->EOF_TOKEN\n"); // Print EOF_TOKEN explicitly if needed

    // Free the tokens array allocated by lex
    free(tokens);
}

// int main()
// {
// 	run_test("|");
// 	// run_test(">");
// 	// run_test("<");
// 	// run_test(">>");
// 	// run_test("<<");
// 	// run_test("=");
// 	// run_test("word");

// 	// run_test(" | ");
// 	// run_test("|	word");
// 	// run_test("|word");
// 	// printf("\n");
// 	// run_test("word |");
// 	// run_test("word|");
// 	// run_test("word | word");
// 	// run_test("| word |");
// 	// run_test("|word |");
// 	// run_test("ls | cat | wc");
//     // run_test("   |    <    >  ");
// 	// run_test(" <  |   >  ");
// 	// run_test("  <    >  |  ");
//     // run_test("| < > << >> =");
// 	// run_test("word");
//     run_test("word1 word2 word3");
//     // run_test("<<word1>> =|");
//     // run_test("1234 5678");
//     // run_test("command1 < input.txt > output.txt | command2");
//     // run_test(">>>>");
// 	// run_test("variable=value");
// 	// run_test("");
// 	// run_test(" ls");
//     // run_test("command -arg");
// 	run_test("command -arg1 -arg2");
// 	run_test("command --arg1");
// 	// run_test("-arg");	
// 	// run_test("#");
//     // run_test("echo \"Hello, World!\"\"\"");
// 	// run_test("echo 'Hello, World!'");
// 	// run_test("echo 'This is a test'");
// 	// run_test("echo \"Another test\"");
// 	// run_test("echo \"This is a \\\"quoted\\\" word\"");
// 	// run_test("echo \"This is a \\\"quoted\\\" word\"");
// 	// run_test("echo 'Single \"double\" quotes'");
// 	// run_test("echo \"Double 'single' quotes\"");
// 	// run_test("A \\| B \\> C \\< D");

// 	// run_test("echo \"Newline:\\nTab:\\t\"");
// 	// run_test("echo \"Nested \\\"quotes\\\" with \\\"escapes\\\"\"");
// 	// run_test("echo \"Unmatched quote test");
// 	// run_test("echo \"$(uname -a)\"");
// 	// run_test("echo \"Complex string: 'Single quotes', \\\"Escaped double quotes\\\", and \\\\escaped backslashes\\\\\"");

// 	// run_test("grep -R \"search term\" ./ | sort | uniq -c > output.txt");

//     return 0;
// }