//TODO: add libft funcs
//TODO: create_node
// #include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

 typedef enum e_token_type
{
	WORD,
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
	N_HEREDOC
}	t_ast_node_type;

typedef struct s_ast_node t_ast_node;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	t_ast_node 			*parent;
	t_ast_node 			*first_child;
	t_ast_node 			*next_sibling;
	t_ast_node 			*last_child;
	char				*data;
	bool				has_file;
	int					fd;
	int 				line;
}	t_ast_node;


typedef struct s_parser_state
{
	t_token		*tokens;
	int			current;
	t_ast_node	*root;
} t_parser_state;

/*
Init the parser state with the tokens to be parsed.
*/
void	init_parser_state(t_parser_state *state, t_token *tokens)
{
	state->tokens = tokens;
	state->current = 0;
	state->root = NULL;
}

/*
Just create node.
*/
t_ast_node	*create_node(t_ast_node_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->parent = NULL;
	node->first_child = NULL;
	node->next_sibling = NULL;
	node->last_child = NULL;
	node->data = NULL;
	node->has_file = false;
	node->fd = -1;
	return (node);
}

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
		return (consume(state), true);
	return (false);
}

/*
Creates nodes for I/O redirections, associating them to the apropriate
tokens.
*/
t_ast_node	*parse_io_redirect(t_parser_state *state)
{
	t_ast_node	*node;
	t_token		target_token;

	node = NULL;
	if(match(state, LESS) || match(state, GREAT))
	{
		// node = create_node(N_IO_REDIRECT);
		target_token = consume(state);
		node->data = strdup(target_token.lexeme);
		node->line = target_token.line;
	}
	return (node);
}

/*
Parse the main word of a command, which is the actual command
*/
t_ast_node	*parse_cmd_word(t_parser_state *state)
{
	t_ast_node	*node;
	t_token		token;

	if (match(state, WORD))
	{
		token = consume(state);
		node = create_node(N_CMD_WORD);
		node->data = strdup(token.lexeme);
		node->line = token.line;
		return (node);
	}
	return (NULL);
}

/*
Parse the prefix of a command, which can be redirections or assignments
*/
// t_ast_node	*parse_cmd_prefix(t_parser_state *state)
// {
// 	t_ast_node	*head;
// 	t_ast_node	*current;
// 	t_ast_node	*new_node;

// 	head = NULL;
// 	current = NULL;
// 	while (match(state, ASSIGNMENT_WORD) || match(state, N_IO_REDIRECT))
// 	{
// 		if (peek(state).type == ASSIGNMENT_WORD)
// 		{
// 			new_node = create_node(ASSIGNMENT_WORD);
// 			new_node->data = strdup(consume(state).lexeme);
// 		}
// 		else
// 		new_node = parse_io_redirect(state);
// 		if (!head)
// 			head = new_node;
// 		else
// 			current->next_sibling = new_node;
// 		current = new_node;	
// 	}
// 	return (head);
// }
t_ast_node *parse_cmd_prefix(t_parser_state *state) {
    t_ast_node *head = NULL;
    t_ast_node *current = NULL;

    while (true) {
        t_token lookahead = peek(state);
        t_ast_node *new_node = NULL;

        if (lookahead.type == ASSIGNMENT_WORD) {
            // Correctly use an AST node type for assignment words
            new_node = create_node(N_CMD_PREFIX); // Assuming N_CMD_PREFIX is used for assignment words
            new_node->data = strdup(lookahead.lexeme);
            consume(state); // Move past the assignment word
        } else if (lookahead.type == LESS || lookahead.type == GREAT) {
            // Handle I/O redirections
            new_node = parse_io_redirect(state); // Ensure parse_io_redirect creates nodes with N_IO_REDIRECT type
        } else {
            // If the token is neither an assignment word nor a redirection, stop parsing the prefix
            break;
        }

        // Link the new node into the prefix chain
        if (!head) {
            head = new_node; // The first node encountered becomes the head of the list
        } else {
            current->next_sibling = new_node; // Link the new node as the next sibling of the current node
        }
        current = new_node; // Update the current pointer to the latest node
    }

    return head;
}



/*
Parse command suffix, which can be redirections or parameters
*/
// t_ast_node	*parse_cmd_suffix(t_parser_state *state)
// {
// 	t_ast_node	*head;
// 	t_ast_node	*current;
// 	t_ast_node	*new_node;

// 	head = NULL;
// 	current = NULL;
// 	while (match(state, WORD) || match(state, N_IO_REDIRECT))
// 	{
// 		new_node = NULL;
// 		if (peek(state).type == WORD)
// 		{
// 			new_node = create_node(N_CMD_PARAM);
// 			new_node->data = strdup(consume(state).lexeme);
// 		}
// 		else
// 			new_node = parse_io_redirect(state);
// 		if(!head)
// 			head = new_node;
// 		else
// 			current->next_sibling = new_node;
// 		current = new_node;
// 	}
// 	return (head);
// }
t_ast_node *parse_cmd_suffix(t_parser_state *state) {
    t_ast_node *head = NULL;
    t_ast_node *current = NULL;

    while (true) {
        t_ast_node *new_node = NULL;
        t_token lookahead = peek(state);

        if (lookahead.type == WORD) {
            // Handle command parameters
            new_node = create_node(N_CMD_PARAM);
            new_node->data = strdup(lookahead.lexeme);
            consume(state); // Consume the WORD token
        }
        else if (lookahead.type == LESS || lookahead.type == GREAT
                 /* Extend with other redirection types as needed, e.g., DLESS, DGREAT */) {
            // Handle I/O redirections
            new_node = parse_io_redirect(state); // Handles consuming the redirection token and its target
        }
        else {
            // Break the loop if the current token is neither a parameter nor a redirection
            break;
        }

        // Link the new node into the suffix chain
        if (!head) {
            head = new_node; // First node becomes the head
        } else {
            current->next_sibling = new_node; // Link new node as the next sibling
        }
        current = new_node; // Update current to the new node
    }

    return head;
}

/*
Parse a single command, which might include a prefix,
a command word and a suffix.
*/
// t_ast_node	*parse_command(t_parser_state *state)
// {
// 	t_ast_node	*node;

// 	node = create_node(N_COMMAND);
// 	if (peek(state).type == ASSIGNMENT_WORD || peek(state).type == WORD)
// 	{
// 		node->first_child = parse_cmd_prefix(state);
// 		node->first_child->next_sibling = parse_cmd_word(state);
// 		node->first_child->next_sibling->next_sibling = parse_cmd_suffix(state);
// 	}
// 	return (node);
// }
t_ast_node *parse_command(t_parser_state *state) {
    t_ast_node *node = create_node(N_COMMAND);
    t_ast_node *last_node = NULL;

    if (peek(state).type == ASSIGNMENT_WORD || peek(state).type == WORD) {
        // Parse parts of the command
        t_ast_node *prefix = parse_cmd_prefix(state);
        t_ast_node *cmd_word = parse_cmd_word(state);
        t_ast_node *suffix = parse_cmd_suffix(state);

        // Link the nodes safely
        if (prefix) {
            node->first_child = prefix;
            last_node = prefix; // The prefix, if present, is the first child
            // Find the last node in the prefix chain to link the cmd_word correctly
            while (last_node->next_sibling != NULL) {
                last_node = last_node->next_sibling;
            }
        }

        if (cmd_word) {
            if (last_node) {
                last_node->next_sibling = cmd_word; // Link cmd_word after the prefix
            } else {
                node->first_child = cmd_word; // No prefix, cmd_word is the first child
            }
            last_node = cmd_word; // Update last_node to cmd_word
        }

        if (suffix) {
            if (last_node) {
                last_node->next_sibling = suffix; // Link suffix after the last node
            } else {
                // This case might not be logically possible based on your grammar
                // since cmd_word should exist if suffix does, but it's here for completeness
                node->first_child = suffix;
            }
        }
    }

    return node;
}


/*
Parse complete command := single command or sequence of commands
separated by pipes.
*/
t_ast_node	*parse_complete_command(t_parser_state *state)
{
	t_ast_node	*node;
	t_ast_node	*current_cmd;

	t_ast_node 	*pipe_node;
	t_ast_node 	*next_cmd;

	while(match(state, PIPE))
	{
		pipe_node = create_node(N_PIPE);
		next_cmd = parse_command(state);

		pipe_node->first_child = current_cmd;
		current_cmd->parent = pipe_node;

		current_cmd->next_sibling = next_cmd;

	}
}

//TODO: diff HEREDOC / FILENAME
/*
Parsing filenames or heredocs, which are used as I/O redirection targets.
	This needs changes, i dont associate the appropriate tokens
*/
t_ast_node *parse_filename_or_heredoc(t_parser_state *state)
{
	t_ast_node *node;
	t_token token;

	token = peek(state);
	if (match(state, WORD))
	{
		// node  = create_node(N_FILENAME);
		node->data = strdup(token.lexeme);
		node->line = token.line;
	}
	return (node);
}

void free_ast(t_ast_node *node) {
    if (node == NULL) {
        return;
    }

    // Free the data associated with the node
    if (node->data != NULL) {
        free(node->data);
    }

    // Recursively free children nodes
    t_ast_node *child = node->first_child;
    while (child != NULL) {
        t_ast_node *next_sibling = child->next_sibling;
        free_ast(child);
        child = next_sibling;
    }

    // Free the node itself
    free(node);
}


/*************************************************************************************/
/****************************************TEST*****************************************/
/*************************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

// t_token ls_tokens [] = {
// 	{WORD, "ls", 1},
// 	{WORD, "-l", 1},
// 	{EOF_TOKEN, NULL, 1}
// };

// t_token echo_tokens [] = {
// 	{WORD, "echo", 1},
// 	{WORD, "hello", 1},
// 	{GREAT, ">", 1},
// 	{WORD, "out_file.txt", 1},
// 	{EOF_TOKEN, NULL, 1}
// };

// t_token cat_tokens [] = {
// 	{WORD, "cat", 1},
// 	{WORD, "in_file.txt", 1},
// 	{PIPE, "|", 1},
// 	{WORD, "grep", 1},
// 	{WORD, "pattern", 1},
// 	{PIPE, "|", 1},
// 	{WORD, "sort", 1},
// 	{GREAT, ">", 1},
// 	{WORD, "out_file.txt", 1},
// 	{EOF_TOKEN, NULL, 1}
// };

const char *nodeTypeToString(t_ast_node_type type) {
    static const char *types[] = {
        "N_COMPLETE_COMMAND",
        "N_COMMAND",
        "N_PIPE",
        "N_CMD_WORD",
        "N_CMD_PREFIX",
        "N_CMD_SUFFIX",
        "N_CMD_PARAM",
        "N_INFILE",
        "N_HEREDOC"
    };
    if (type >= 0 && type < sizeof(types) / sizeof(types[0])) {
        return types[type];
    } else {
        return "UNKNOWN_TYPE";
    }
}

void print_ast(const t_ast_node *node, int depth)
{
	if (!node) return;
	for (int i = 0; i < depth; ++i) {
		printf("  ");
	}
	printf("%s", nodeTypeToString(node->type));
	if (node->data) {
		printf(" (Data: %s)", node->data);
	}
	printf("\n");
	if (node->first_child)
		print_ast(node->first_child, depth + 1);
	if (node->next_sibling)
		print_ast(node->next_sibling, depth);
}



/*
Simple word.
*/

void test_parse_word()
{
    t_token word_tokens[] = {
        {WORD, "cd", 1},
        {EOF_TOKEN, NULL, 1}
    };

    t_parser_state state;
    init_parser_state(&state, word_tokens);

    t_ast_node *node = parse_command(&state);

    printf("Running tests...\n");
    print_ast(node, 0);

    if (node == NULL || node->type != N_COMMAND || strcmp(node->first_child->data, "cd") != 0) {
        printf("Test failed.\n");
        exit(1);
    } else {
        printf("Test passed.\n");
    }

    free_ast(node);
}

/*
Command with argument.
*/
void test_command_with_argument() {
    t_token tokens[] = {
        {WORD, "ls", 1},
        {WORD, "-la", 1},
        {EOF_TOKEN, NULL, 1}
    };

    t_parser_state state;
    init_parser_state(&state, tokens);

    t_ast_node *node = parse_command(&state);
    printf("Running test_command_with_argument...\n");
    print_ast(node, 0);

    // Check if the root node and command node are structured correctly
    if (node == NULL || node->type != N_COMPLETE_COMMAND || node->type != N_COMMAND) {
        printf("Test failed: Root node is not N_COMPLETE_COMMAND.\n");
        exit(1);
    }

    // Assuming the command node is the first child of the root node
    t_ast_node *commandNode = node->first_child;
    if (commandNode == NULL || commandNode->type != N_COMMAND || strcmp(commandNode->data, "ls") != 0) {
        printf("Test failed: Command node incorrect or missing.\n");
        exit(1);
    }

    // Assuming arguments are siblings of the command node or its children
    t_ast_node *argNode = commandNode->next_sibling; // If arguments are siblings
    // OR for child approach: t_ast_node *argNode = commandNode->first_child; // If arguments are children
    if (argNode == NULL || strcmp(argNode->data, "-la") != 0) {
        printf("Test failed: Argument node incorrect or missing.\n");
        exit(1);
    }

    printf("Test passed.\n");

    free_ast(node);
}

int main()
{
	test_parse_word();
	printf("____________________\n");
	test_command_with_argument();
	return (0);
}