//TODO: add libft funcs
//TODO: create_node
// #include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum e_token_type
{
	WORD,
	OPTION,
	ASSIGNMENT_WORD,
	DLESS,
	DGREAT,
	LESS,
	GREAT,
	PIPE,
	EOF_TOKEN ,
	ERROR_TOKEN
}	t_token_type;


typedef struct s_token
{
  t_token_type	type;
  char			*lexeme;
  int			line;
} t_token;

typedef enum e_ast_node_type
{
	N_COMPLETE_COMMAND,
	N_COMMAND,
	N_PIPE,
	N_CMD_WORD,
	N_CMD_PREFIX,
	N_CMD_SUFFIX,
	N_CMD_PARAM,
	N_INFILE,
	N_HEREDOC,
	N_ERROR
}	t_ast_node_type;

typedef struct s_ast_node t_ast_node;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	t_ast_node 			*parent;
	t_ast_node 			*left;
	t_ast_node 			*right;
	char				*data;
	bool				has_file;
	int					fd;
	int 				line;
}	t_ast_node;

typedef struct s_parser_state
{
	t_token		*tokens;
	int			current;
} t_parser_state;


//UTIL FUNCTIONS
/*
This function returns the current token form the parser state,
without advancing the current position(look-ahead)
*/
t_token	peek(t_parser_state *state)
{
	return(state->tokens[state->current]);
}

/*
This function advances the parser state to the next token and
returns the token that was consumed. It's used to consume a 
token when it matches the expected type and the machine is 
ready to move on to the next one.
*/
t_token	consume(t_parser_state *state)
{
	if (peek(state).type != EOF_TOKEN)
		++state->current;
	return (state->tokens[state->current - 1]);
}

/*
This function checks if the current token matches the expected type.
*/
bool	match(t_parser_state *state, t_token_type type)
{
	if (peek(state).type == type)
		return (consume(state), true); // Advance to the next token
	return (false);
}

void	init_parser_state(t_parser_state *state, t_token *tokens)
{
	state->tokens = tokens;
	state->current = 0;
}

t_ast_node	*create_node(t_ast_node_type type)
{
	t_ast_node *node;

	node = malloc(sizeof(t_ast_node));
	node->type = type;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	node->data = NULL;
	node->has_file = false;
	node->fd = -1;
	node->line = -1;
	return (node);
}
t_ast_node_type determine_node_type(t_token_type type) {
	if (type == WORD)
		return N_CMD_WORD;
	else if (type == OPTION)
		return N_CMD_PARAM;
	else if (type == ASSIGNMENT_WORD)
		return N_CMD_PARAM;
	else if (type == GREAT || type == LESS \
		|| type == DLESS || type == DGREAT)
		return N_INFILE;
	else {
		printf("Unknown token type encountered.\n");
		return N_ERROR;
	}
}

t_ast_node	*parse_cmd_suffix(t_parser_state *state)
{
	t_ast_node	*head = NULL;
	t_ast_node	*current = NULL;
	t_ast_node	*node = NULL;

	while (peek(state).type == ASSIGNMENT_WORD || peek(state).type == GREAT \
		|| peek(state).type == LESS || peek(state).type == DLESS \
		|| peek(state).type == DGREAT || peek(state).type == OPTION \
		|| peek(state).type == WORD)
	{
		node = create_node(determine_node_type(peek(state).type));
		node->data = strdup(peek(state).lexeme);
		if (!head)
			head = node;
		else
			current->right = node;
		current = node;
		consume(state);
	}
	return (head);
}

t_ast_node	*parse_cmd_prefix(t_parser_state *state)
{
	printf("going in parse_cmd_prefix\n");
	t_ast_node *head = NULL;
	t_ast_node *current = NULL;
	t_ast_node *node = NULL;

	while(peek(state).type == ASSIGNMENT_WORD || peek(state).type == GREAT \
		|| peek(state).type == LESS || peek(state).type == DLESS \
		|| peek(state).type == DGREAT)
	{
		node = create_node(determine_node_type(peek(state).type));
		node->data = strdup(peek(state).lexeme);
		printf("Created node: %d with data %s\n", node->type, node->data);
		if (!head)
			head = node;
		else
			current->right = node;
		current = node;
		consume(state);
	}
	return (NULL);
}
t_ast_node	*parse_command(t_parser_state *state)
{
	t_ast_node *cmd;
	t_ast_node *cmd_prefix = NULL;
	t_ast_node *cmd_suffix = NULL;
	t_ast_node *cmd_word = NULL;
	t_token consumed;

	cmd = create_node(N_COMMAND);
	
	// Parse cmd_prefix if present
	if (peek(state).type == ASSIGNMENT_WORD || peek(state).type == GREAT \
		|| peek(state).type == LESS || peek(state).type == DLESS \
		|| peek(state).type == DGREAT)
		cmd_prefix = parse_cmd_prefix(state);
	// Parse cmd_word if present
	if (cmd_prefix || peek(state).type == WORD)
	{
		cmd_word = create_node(N_CMD_WORD);
		if (!cmd_prefix && peek(state).type == WORD)
		{
			cmd_word->data = strdup(peek(state).lexeme);
			consume(state);
		}
		else if (cmd_prefix)
		{
			// If there's a prefix, the next token is assumed to be a cmd_word, so consume it
			consumed = consume(state);
			cmd_word->data = strdup(consumed.lexeme);
		}
printf(" Befilinking prefix:\n");
printf("Cmd Prefix: %s\n", cmd_prefix ? cmd_prefix->data : "None");

		if (cmd_prefix)
		{
			cmd_prefix->right = cmd_word;
			cmd->left = cmd_prefix;
		} 
		else
			cmd->left = cmd_word;
printf("After linking prefix:\n");
printf("Cmd Prefix: %s\n", cmd_prefix ? cmd_prefix->data : "None");
	}
	// Parse cmd_suffix if present
	if (peek(state).type == ASSIGNMENT_WORD || peek(state).type == GREAT \
		|| peek(state).type == LESS || peek(state).type == DLESS \
		|| peek(state).type == DGREAT || peek(state).type == OPTION \
		|| peek(state).type == WORD)
	{
		cmd_suffix = parse_cmd_suffix(state);
		// Directly attach cmd_suffix without conditionals since all branches do the same
		cmd->right = cmd_suffix;
	}
	return (cmd);
}


t_ast_node	*parse_complete_command(t_parser_state *state)
{
	t_ast_node	*command;
	t_ast_node	*next_cmd;
	t_ast_node 	*error;
	t_ast_node 	*pipe;

	command = parse_command(state);
	if (match(state, PIPE))
	{
		next_cmd = parse_complete_command(state);
		pipe = create_node(N_PIPE);
		pipe->data = strdup("|");//rm this
		pipe->left = command;
		pipe->right = next_cmd;
		return (pipe);
	}
	else
		if (peek(state).type == EOF_TOKEN)
			return (command);
		else
		{
			error = create_node(N_ERROR);
			error->left = command;
			return (error);
		}
		// return (command);
}

t_ast_node	*parse_input(const char *input)
{
	t_token	*tokens;
	t_parser_state	state;
	t_ast_node	*ast;

	// tokens = lex(input);
	init_parser_state(&state, tokens);
	ast = parse_complete_command(&state);

	return (ast);
}


/**************************TEST**************************************/



// Mock function to create an array of tokens for testing
t_token *create_mock_tokens() {
    static t_token tokens[] = {
		// {WORD, "export", 0},
        // {ASSIGNMENT_WORD, "VAR=value", 0},
        // {WORD, "echo", 0},
		// {WORD, "$VAR", 0},
        // {WORD, "Hello, world!", 0},
		{WORD,"ls", 0},
		{OPTION,"-l", 0},
		{PIPE, "|", 0},
		{WORD,"wc", 0},
		{PIPE, "|", 0},
		{WORD,"grep", 0},
		// {WORD, "grep", 0},
		{WORD, "test", 0},
        {EOF_TOKEN, NULL, 0}
    };
    return tokens;
}

void print_ast(t_ast_node *node, int level)
{
	if (node == NULL) return;
	for (int i = 0; i < level; ++i) printf("  ");
	printf("%d: %s\n", node->type, node->data ? node->data : "NULL");
	print_ast(node->left, level + 1);
	print_ast(node->right, level + 1);
}

int main() {
    t_token *tokens = create_mock_tokens();
    t_parser_state state;
    init_parser_state(&state, tokens);

    t_ast_node *ast = parse_complete_command(&state);
    print_ast(ast, 0);

    // Don't forget to free your AST and tokens after testing
    // (not shown here for brevity)

    return 0;
}
