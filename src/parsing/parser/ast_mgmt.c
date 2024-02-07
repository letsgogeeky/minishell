//TODO: add libft funcs
//TODO: create_node
// #include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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


typedef struct s_token
{
  t_token_type	type;
  char			*lexeme;
  int			line;
} t_token;

typedef enum e_ast_node_type
{
	COMPLETE_COMMAND,
	COMMAND,
	PIPE,
	CMD_WORD,
	CMD_PREFIX,
	CMD_SUFFIX,
	CMD_PARAM,
	IO_REDIRECT,
}	t_ast_node_type;

// typedef struct s_ast_node t_ast_node;

// typedef union u_node_data
// {
// 	struct
// 	{
// 		t_ast_node *left;
// 		t_ast_node *right;
// 	}	binary;
// 	char *text;
// } t_node_data; // a + b

typedef struct s_ast_node
{
	t_ast_node_type		type;
	t_ast_node 			*parent;
	t_ast_node 			*first_child;
	t_ast_node 			*next_sibling;
	t_ast_node 			*last_child;
	char				*data; //-->t_node_data
	char				*filename;
	int 				line;
}	t_ast_node;


typedef struct s_parser_state
{
	t_token		*tokens;
	int			current;
	t_ast_node	*root;
} t_parser_state;

void	init_parser_state(t_parser_state *state, t_token *tokens)
{
	state->tokens = tokens;
	state->current = 0;
	state->root = NULL;
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
This function advances the parser satte to the next token and
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
Parse complete command := single command or sequence of commands
separated by pipes.
*/
t_ast_node	*parse_complete_command(t_parser_state *state)
{
	t_ast_node	*node;
	t_ast_node	*current_cmd;
	t_ast_node 	*pipe_node;
	t_ast_node 	*next_cmd;

	node = create_node(COMPLETE_COMMAND);
	node->data = NULL;
	current_cmd = parse_command(state);
	node->first_child = current_cmd;
	while (match(state, PIPE))
	{
		pipe_node = create_node(PIPE);
		next_cmd = parse_command(state);
		pipe_node->first_child = current_cmd;
		pipe_node->next_sibling = next_cmd;
		current_cmd = next_cmd;
	}
	return (node);
}

/*
Parse a single command, which might include a prefix,
a command word and a suffix.
*/
t_ast_node	*parse_command(t_parser_state *state)
{
	t_ast_node	*node;

	node = create_node(COMMAND);
	if (peek(state).type == ASSIGNMENT_WORD || peek(state).type == WORD)
	{
		node->first_child = parse_cmd_prefix(state);
		node->first_child->next_sibling = parse_cmd_word(state);
		node->first_child->next_sibling->next_sibling = parse_cmd_suffix(state);
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
		node = create_cmd_word(token.lexeme);
		node->line = token.line;
		return (node);
	}
	return (NULL);
}

/*
Parse the prefix of a command, which can be redirections or assignments
*/
t_ast_node	*parse_cmd_prefix(t_parser_state *state)
{
	t_ast_node	*head;
	t_ast_node	*current;
	t_ast_node	*new_node;

	head = NULL;
	current = NULL;
	while (match(state, ASSIGNMENT_WORD) || match(state, IO_REDIRECT))
	{
		if (peek(state).type == ASSIGNMENT_WORD)
		{
			new_node = create_node(ASSIGNMENT_WORD);
			new_node->data = strdup(consume(state).lexeme);
		}
		else
		new_node = parse_io_redirect(state);
		if (!head)
			head = new_node;
		else
			current->next_sibling = new_node;
		current = new_node;	
	}
	return (head);
}

/*
Parse command suffix, which can be redirections or parameters
*/t_ast_node	*parse_cmd_suffix(t_parser_state *state)
{
	t_ast_node	*head;
	t_ast_node	*current;
	t_ast_node	*new_node;

	head = NULL;
	current = NULL;
	while (match(state, WORD) || match(state, IO_REDIRECT))
	{
		new_node = NULL;
		if (peek(state).type == WORD)
		{
			new_node = create_node(CMD_PARAM);
			new_node->data = strdup(consume(state).lexeme);
		}
		else
			new_node = parse_io_redirect(state);
		if(!head)
			head = new_node;
		else
			current->next_sibling = new_node;
		current = new_node;
	}
	return (head);
}

/*
Creates nodes for I/O redirections, associating them to the apropriate
tokens.
*/
t_ast_node	*parse_io_redirect(t_parser_state *state)
{
	t_ast_node	*node;
	
	if(match(state, LESS) || match(state, GREAT))
	{
		create_node(IO_REDIRECT);
		node->data = strdup(consume(state).lexeme); //redir target should imediately follow?
	}
	return (node);
}
/*
Parsing filenames or heredocs, which are used as I/O redirection targets.
	This needs changes, i dont associate the appropriate tokens
*/
t_ast_node *parse_filename_or_heredoc(t_parser_state *state)
{
	t_ast_node *node;
	t_token token;

	if (match(state, WORD))
	{
		token = consume(state);
		node = create_node(CMD_PARAM);
		node->data = strdup(token.lexeme);
		node->line = token.line;
	}
	else
	{
		node = create_node(DLESS);
		node->data = strdup("<<");
	}
	return (node);
}

// t_ast_node *ast_new_node(t_ast_node_type type, char *value)
// {
// 	t_ast_node *node;

// 	node = malloc(sizeof(t_ast_node));
// 	if (!node)
// 		return (NULL);

// 	node->type = type;
// 	node->parent = NULL;
// 	node->first_child = NULL;
// 	node->next_sibling = NULL;
// 	node->last_child = NULL;
// 	node->data = strdup(value);
// 	if (!node->data)
// 		return(free(node), NULL);
// 	node->filename = NULL;
// 	return (node);
// }

