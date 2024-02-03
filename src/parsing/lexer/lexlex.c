// #include "minishell/parsing/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef enum e_token_type
{
	WORD ,
	ASSIGNMENT_WORD,
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

	START = 9,
	IN_WORD,
	IN_SEPARATOR,
	IN_DLESS,
	IN_DGREAT,
	IN_LESS,
	IN_GREAT,
	IN_PIPE,
	IN_ASSIGNMENT,
	IN_ERROR
}	t_state;

typedef struct s_token
{
  t_token_type	type;
  char			*lexeme;
  int			line;
} t_token;

typedef struct s_lexer
{
	char	*input_buffer;
	char	*current;
	char	*start;
	int		line_number;
	t_state	state;
}	t_lexer;

void	init_lexer(t_lexer *lexer, const char *input)
{
	lexer->input_buffer = strdup(input);
	if (!lexer->input_buffer)
	{
		fprintf(stderr, "Failed to allocate memory for input buffer\n");
		exit(1);
	}
	lexer->current = lexer->input_buffer;
	lexer->start = lexer->input_buffer;
	lexer->line_number = 1;
	lexer->state = START;
}

// void	advance_lexer(t_lexer *lexer)
// {
// 	if (*lexer->current == '\n')
// 		lexer->line_number++;
// 	lexer->current++;
// }

void	destroy_lexer(t_lexer *lexer)
{
	free(lexer->input_buffer);
}

t_token *new_token(t_token_type type, t_lexer *lexer)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
	{
		fprintf(stderr, "Failed to allocate memory for token\n");
		exit(1);
	}
	token->type = type;
	token->lexeme = strndup(lexer->start, lexer->current - lexer->start);
	token->line = lexer->line_number;
	// printf("Generating token: %d, Lexeme: '%.*s'\n", type, (int)(lexer->current - lexer->start), lexer->start);
	return (token);
}

void	update_state(t_lexer *lexer, char curr_ch)
{
	if (isspace(curr_ch))
		lexer->state = IN_SEPARATOR;
	else if (curr_ch == '|')
		lexer->state = IN_PIPE;
	else if (curr_ch == '<' && *(lexer->current + 1) != '<')
		lexer->state = IN_LESS;
	else if (curr_ch == '>' && *(lexer->current + 1) != '>')
		lexer->state = IN_GREAT;
	else if (curr_ch == '<' && *(lexer->current + 1) == '<')
		lexer->state = IN_DLESS;
	else if (curr_ch == '>' && *(lexer->current + 1) == '>')
		lexer->state = IN_DGREAT;
	else if (curr_ch == '=')
		lexer->state = IN_ASSIGNMENT;
	else if (curr_ch == '\n')
		lexer->state = START;
	else if (isalpha(curr_ch) || isdigit(curr_ch)|| curr_ch == '_')
		lexer->state = IN_WORD;
	else
		lexer->state = IN_ERROR;
}

t_token	*get_next_token(t_lexer *lexer)
{
	while (*lexer->current)
	{
		update_state(lexer, *lexer->current);
		if (lexer->state == IN_WORD)
		{
			lexer->start = lexer->current;
			while (isalnum(*lexer->current) || *lexer->current == '_')
				++lexer->current;
			return (new_token(WORD, lexer));
		}
		else if (lexer->state == IN_PIPE)
		{
			lexer->start = lexer->current;
			++lexer->current;
			return (new_token(PIPE, lexer));
		}
		else if (lexer->state == IN_LESS)
		{
			lexer->start = lexer->current;
			++lexer->current;
			return (new_token(LESS, lexer));
		}
		else if (lexer->state == IN_GREAT)
		{
			lexer->start = lexer->current;
			++lexer->current;
			return (new_token(GREAT, lexer));
		}
		else if (lexer->state == IN_DLESS)
		{
			lexer->start = lexer->current;
			lexer->current += 2;
			return (new_token(DLESS, lexer));
		}
		else if (lexer->state == IN_DGREAT)
		{
			lexer->start = lexer->current;
			lexer->current += 2;
			return (new_token(DGREAT, lexer));
		}
		else if (lexer->state == IN_ASSIGNMENT)
		{
			lexer->start = lexer->current;
			++lexer->current;
			return (new_token(ASSIGNMENT_WORD, lexer));
		}
		else if (lexer->state == IN_SEPARATOR)
		{
			++lexer->current;
			continue;
		}
		else if (lexer->state == IN_ERROR)
		{
			++lexer->current;
			return (new_token(ERROR_TOKEN, lexer));
		}
	}
	return (new_token(EOF_TOKEN, lexer));
}


//<TESTS>//

/*************************************************************************************/
/********************************NEW_TOKEN TESTS**************************************/
/*************************************************************************************/
// #include <assert.h>

// void	test_simple_word_token(void)
// {
// 	t_lexer lexer;
// 	init_lexer(&lexer, "hello");

// 	lexer.current = lexer.input_buffer + strlen(lexer.input_buffer);
// 	t_token *token = new_token(WORD, &lexer);

// 	assert(token->type == WORD);
// 	assert(strcmp(token->lexeme, "hello") == 0);

// 	printf("Test 1 Passed: Simple Word Token\n");
	
// 	free(token->lexeme); // Free the lexeme strdup'ed in new_token
// 	free(token);         // Free the token itself
// 	destroy_lexer(&lexer); // Free lexer resources, corrected function name
// }

// void test_special_character_token()
// {
//     t_lexer lexer;
//     init_lexer(&lexer, "|");
    
// 	lexer.current++;
//     t_token *token = new_token(PIPE, &lexer);

// 	assert(token->type == PIPE);
//     assert(strcmp(token->lexeme, "|") == 0);
    
//     printf("Test 2 Passed: Special Character Token (PIPE)\n");
    
//     free(token->lexeme);
//     free(token);
//     destroy_lexer(&lexer);
// }

// void test_assignment_word_token()
// {
//     t_lexer lexer;
//     init_lexer(&lexer, "variable=value");
    
//     lexer.current = lexer.input_buffer + strlen("variable=value");
    
//     t_token *token = new_token(ASSIGNMENT_WORD, &lexer);

//     assert(token->type == ASSIGNMENT_WORD);
//     assert(strcmp(token->lexeme, "variable=value") == 0);
    
//     printf("Test 3 Passed: Assignment Word Token\n");
    
//     free(token->lexeme);
//     free(token);
//     destroy_lexer(&lexer);
// }

// void test_empty_lexeme()
// {
//     t_lexer lexer;
//     init_lexer(&lexer, "");
    
//     // lexer->start and lexer->current are both at the beginning of the empty string
//     t_token *token = new_token(EOF_TOKEN, &lexer);
    
//     assert(token->type == EOF_TOKEN);
//     assert(strcmp(token->lexeme, "") == 0);
    
//     printf("Test 4 Passed: Empty Lexeme\n");
    
//     free(token->lexeme);
//     free(token);
//     destroy_lexer(&lexer);
// }

// void test_multibyte_character_handling()
// {
//     t_lexer lexer;
//     init_lexer(&lexer, "こんにちは"); // "Hello" in Japanese
    
//     lexer.current = lexer.input_buffer + strlen("こんにちは");
    
//     t_token *token = new_token(WORD, &lexer);
    
//     assert(token->type == WORD);
//     assert(strcmp(token->lexeme, "こんにちは") == 0);
    
//     printf("Test 5 Passed: Multi-Byte Character Handling\n");
    
//     free(token->lexeme);
//     free(token);
//     destroy_lexer(&lexer);
// }

// void test_consecutive_special_characters() {
//     t_lexer lexer;
//     init_lexer(&lexer, "|||");

//     for (int i = 0; i < 3; ++i) {
//         // Advance lexer.current to the next character for token generation
//         lexer.current++;

//         // Adjust lexer.start to point to the current character for correct lexeme capture
//         lexer.start = lexer.current - 1; // Point lexer.start to the current PIPE character

//         t_token *token = new_token(PIPE, &lexer);

//         printf("Token type: %d, Expected type: %d\n", token->type, PIPE);
//         printf("Token lexeme: '%s', Expected lexeme: '|'\n", token->lexeme);

//         assert(token->type == PIPE);
//         assert(strcmp(token->lexeme, "|") == 0);

//         free(token->lexeme);
//         free(token);
//     }

//     printf("Test 6 Passed: Multiple Consecutive Special Characters\n");
    
//     destroy_lexer(&lexer);
// }


// void test_word_followed_by_special_character() {
//     t_lexer lexer;
//     init_lexer(&lexer, "word|");

//     // Advance lexer.current to the end of "word"
//     while (*lexer.current != '|') lexer.current++;
//     t_token *word_token = new_token(WORD, &lexer);

//     assert(word_token->type == WORD);
//     assert(strcmp(word_token->lexeme, "word") == 0);

//     free(word_token->lexeme);
//     free(word_token);

//     // Correctly position lexer.start for the PIPE token
//     lexer.start = lexer.current; // Ensure lexer.start points to the '|' character

//     // Generate PIPE token
//     lexer.current++; // Advance past '|'
//     t_token *pipe_token = new_token(PIPE, &lexer);

//     assert(pipe_token->type == PIPE);
//     assert(strcmp(pipe_token->lexeme, "|") == 0);

//     printf("Test 7 Passed: Word Followed by a Special Character\n");

//     free(pipe_token->lexeme);
//     free(pipe_token);
//     destroy_lexer(&lexer);
// }

// void test_newline_character_handling() {
//     t_lexer lexer;
//     init_lexer(&lexer, "line1\nline2");

//     // Advance lexer.current to the end of "line1"
//     while (*lexer.current != '\n') lexer.current++;
//     t_token *line1_token = new_token(WORD, &lexer);

//     assert(line1_token->type == WORD);
//     assert(strcmp(line1_token->lexeme, "line1") == 0);
    
//     free(line1_token->lexeme);
//     free(line1_token);

//     lexer.current++; // Advance past '\n', simulate line number increment
//     lexer.line_number++;

//     // Generate token for "line2"
//     lexer.start = lexer.current; // Update start to the beginning of "line2"
//     while (*lexer.current != '\0') lexer.current++; // Advance to the end of "line2"
//     t_token *line2_token = new_token(WORD, &lexer);

//     assert(line2_token->type == WORD);
//     assert(strcmp(line2_token->lexeme, "line2") == 0);
//     assert(line2_token->line == 2); // Ensure line number is correctly incremented

//     printf("Test 8 Passed: Newline Character Handling\n");

//     free(line2_token->lexeme);
//     free(line2_token);
//     destroy_lexer(&lexer);
// }

// int main()
// {
// 	// 1: Simple Word Token
// 	test_simple_word_token();
// 	// 2: Special Character Token
// 	test_special_character_token();
// 	// 3: Assignment Word Token
// 	test_assignment_word_token();
// 	// 4: Empty Lexeme
// 	test_empty_lexeme();
// 	// 5: Multi-Byte Character Handling
// 	test_multibyte_character_handling(); //passed
// 	// 6: Consecutive Special Characters
// 	test_consecutive_special_characters();
// 	// 7: Word Followed by a Special Character
// 	test_word_followed_by_special_character();
// 	// 8: Newline Character Handling
// 	test_newline_character_handling();

// 	return 0;
// }

/*************************************************************************************/
/*************************************************************************************/
/*************************************************************************************/



// Testing

void print_token(const t_token* token) {
    switch (token->type) {
        case WORD: printf("WORD(%s) ", token->lexeme); break;
        case ASSIGNMENT_WORD: printf("ASSIGNMENT_WORD(%s) ", token->lexeme); break;
        case DLESS: printf("DLESS "); break;
        case DGREAT: printf("DGREAT "); break;
        case LESS: printf("LESS "); break;
        case GREAT: printf("GREAT "); break;
        case PIPE: printf("PIPE "); break;
        case EOF_TOKEN: printf("EOF_TOKEN "); break;
        case ERROR_TOKEN: printf("ERROR_TOKEN "); break;
        default: printf("UNKNOWN ");
    }
}

void run_test(const char *test_input) {
    t_lexer lexer;
    init_lexer(&lexer, test_input);

    printf("Test Input: \"%s\"\n", test_input);
    printf("Tokens: ");
    t_token *token;
    while ((token = get_next_token(&lexer))->type != EOF_TOKEN) {
		printf("-->");
        print_token(token);
        free(token);
    }
	printf("-->");
    print_token(token); // Print EOF_TOKEN
    free(token);

    destroy_lexer(&lexer);
    printf("\n");
}

int main()
{
	//TEST:BASIC -->PASSED
	// run_test("|");
	// run_test(">");
	// run_test("<");
	// run_test(">>");
	// run_test("<<");
	// run_test("=");
	// run_test("word");

	// run_test(" | ");
	// run_test("|	word");
	// run_test("|word");
	// printf("\n");
	// run_test("word |");
	// run_test("word|");
	// run_test("word | word");
	// run_test("| word |");
	// run_test("|word |");
	// run_test("ls | cat | wc");
    // run_test("   |    <    >  ");
	// run_test(" <  |   >  ");
	// run_test("  <    >  |  ");
    // run_test("| < > << >> =");
	
	// TEST
	// run_test("word");
    // run_test("word1 word2 word3");
    // run_test("<<word1>> =|");
    // run_test("1234 5678");
    // run_test("command1 < input.txt > output.txt | command2");
    // run_test(">>>>");
    // run_test("command -arg");
    // run_test("variable=value");
    // run_test("");
    // run_test("#");
    // run_test("echo \"Hello, World!\"");
	// run_test(" ls");
    // run_test("if [ -d $directory ]; then echo 'Exists'; fi");
    return 0;
}