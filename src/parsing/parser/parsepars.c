//TODO: add libft funcs

// #include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

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
	N_COMMAND,
	N_PIPE,
	N_CMD_WORD,
	N_CMD_PREFIX,
	N_CMD_SUFFIX,
	N_CMD_PARAM,
	N_INFILE,
	N_OUTFILE,
	N_HEREDOC,
	N_ERROR
}	t_ast_node_type;

typedef struct s_ast_node t_ast_node;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	t_ast_node 			*parent;
	t_ast_node 			*child;
	t_ast_node			*sibling;
	char				*data;
	bool				is_heredoc;
	int					fd;
}	t_ast_node;

typedef struct s_parser_state
{
	t_token		*tokens;
	int			current;
} t_parser_state;

int error_file_not_found(char *path)
{
    // ft_putstr_fd("minishell: ", STDERR_FILENO);
    // ft_putstr_fd(path, STDERR_FILENO);
    // ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	fprintf(stderr, "minishell: %s: No such file or directory\n", path);
    return (-1);
}

int error_permission_denied(char *path)
{
    // ft_putstr_fd("minishell: ", STDERR_FILENO);
    // ft_putstr_fd(path, STDERR_FILENO);
    // ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	fprintf(stderr, "minishell: %s: Permission denied\n", path);
    return (-1);
}

int open_file(char *path, int flags)
{
    int fd;

    if (flags == O_RDONLY && access(path, F_OK) == -1)
        return (error_file_not_found(path));
    if (flags == O_RDONLY && access(path, R_OK) == -1)
        return (error_permission_denied(path));
    if (flags > O_RDONLY && !access(path, F_OK) && access(path, W_OK) == -1)
        return (error_permission_denied(path));
    if (flags == O_RDONLY)
        fd = open(path, flags);
    else
        fd = open(path, O_WRONLY | O_CREAT | flags, 0644);
    return (fd);
}


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

// Init funcs
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
	node->child = NULL;
	node->sibling = NULL;
	node->data = NULL;
	node->is_heredoc = false;
	node->fd = -1;
	return (node);
}
void destroy_ast(t_ast_node *node)
{
	if (!node)
		return;
	destroy_ast(node->child);
	destroy_ast(node->sibling);
	free(node->data);
	free(node);
}

//Utils
t_ast_node_type determine_node_type(t_token_type type)
{
	if (type == WORD)
		return (N_CMD_WORD);
	else if (type == OPTION)
		return (N_CMD_PARAM);
	else if (type == ASSIGNMENT_WORD)
		return (N_CMD_PARAM);
	else if (type == GREAT || type == LESS \
		|| type == DLESS || type == DGREAT)
		return (N_INFILE);
	else
		return (printf("Unknown token type encountered.\n"), \
			N_ERROR);
}

bool	is_redirect(t_parser_state *state)
{
	t_token_type type;

	type = peek(state).type;
	return(type == GREAT || type == LESS \
		|| type == DLESS || type == DGREAT);
}

t_ast_node *parse_redirect(t_parser_state *state)
{
	t_ast_node 		*node;
	t_token_type	type;
	t_token			file;

	type = peek(state).type;
	consume(state);
	file = consume(state);
	node = create_node(determine_node_type(type));
	node->data = strdup(file.lexeme);
	if (type == GREAT)
		node->fd = open_file(file.lexeme, O_TRUNC);
	else if (type == DGREAT)
		node->fd = open_file(file.lexeme, O_APPEND);
	else if (type == LESS)
		node->fd = open_file(file.lexeme, O_RDONLY);
	else if (type == DLESS)
		node->is_heredoc = true;
	if (node->fd == -1)
		exit(EXIT_FAILURE);
	return (node);
}

//TODO:parse_prefix/suffix merge somehow

t_ast_node *parse_cmd_suffix(t_parser_state *state)
{
    t_ast_node *head = NULL;
    t_ast_node *current = NULL;
    t_ast_node *node = NULL;

	while (peek(state).type == ASSIGNMENT_WORD || is_redirect(state) \
		|| peek(state).type == OPTION || peek(state).type == WORD)
	{
		if(is_redirect(state))
			node = parse_redirect(state);
		else
		{
        	node = create_node(determine_node_type(peek(state).type));
			node->data = strdup(peek(state).lexeme);
		}
		if (!head)
			head = node;
		else
			current->sibling = node;
		current = node;
		consume(state);
	}
	return (head);
}


t_ast_node *parse_cmd_prefix(t_parser_state *state)
{
	t_ast_node *head = NULL;
	t_ast_node *current = NULL;
	t_ast_node *node = NULL;

	while(is_redirect(state) || peek(state).type == ASSIGNMENT_WORD)
	{
		if (is_redirect(state))
			node = parse_redirect(state);
		else if (peek(state).type == ASSIGNMENT_WORD)
		{
			node = create_node(determine_node_type(peek(state).type));
			node->data = strdup(peek(state).lexeme);
		}
		if (!head)
			head = node;
		else
			current->sibling = node;
		current = node;
		consume(state);
	}
	return (head);
}


t_ast_node *parse_command(t_parser_state *state)
{
	t_ast_node *cmd = create_node(N_COMMAND);
	t_ast_node *last_child = NULL;
	t_ast_node *cmd_suffix = NULL;
	t_ast_node *cmd_prefix = NULL;
	t_ast_node *cmd_word = NULL;

	if (is_redirect(state) || peek(state).type == ASSIGNMENT_WORD)
	{
		cmd_prefix = parse_cmd_prefix(state);
		cmd->child = cmd_prefix;
		last_child = cmd_prefix;
		while (last_child->sibling != NULL)
			last_child = last_child->sibling;
	}
	if (peek(state).type == WORD)
	{
	cmd_word = create_node(N_CMD_WORD);
		cmd_word->data = strdup(peek(state).lexeme);
		consume(state);
		if (last_child)
			last_child->sibling = cmd_word;
		else
			cmd->child = cmd_word;
		last_child = cmd_word;
	}
	if (peek(state).type == ASSIGNMENT_WORD || is_redirect(state) \
		|| peek(state).type == WORD)
	{
		cmd_suffix = parse_cmd_suffix(state);
		if (last_child)
			last_child->sibling = cmd_suffix;
		else
			cmd->child = cmd_suffix;
	}
	return (cmd);
}



t_ast_node *parse_complete_command(t_parser_state *state)
{
	t_ast_node *command;
	t_ast_node *pipe;
	t_ast_node *next_cmd;
	t_ast_node *error;

	command = parse_command(state);
	if (match(state, PIPE))
	{
		pipe = create_node(N_PIPE);
		pipe->data = strdup("|"); //TODO: remove this
		next_cmd = parse_complete_command(state);
		pipe->child = command;
		command->parent = pipe;
		if (next_cmd)
		{
			next_cmd->parent = pipe;
			command->sibling = next_cmd;
		}
		return (pipe);
	}
	else if (peek(state).type == EOF_TOKEN)
		return (command);
	else
	{
		error = create_node(N_ERROR);
		error->child = command;
		command->parent = error;
		return (error);
	}
}


t_ast_node	*parse_input(const char *input)
{
	t_token			*tokens;
	t_parser_state	state;
	t_ast_node		*ast;

	// tokens = lex(input);
	init_parser_state(&state, tokens);
	ast = parse_complete_command(&state);

	return (ast);
}



/********************************************************************/
/**************************TEST**************************************/
/********************************************************************/


// Mock function to create an array of tokens for testing
t_token *create_mock_tokens() {
    static t_token tokens[] = {
		// {WORD, "export", 0},
        // {ASSIGNMENT_WORD, "VAR=value", 0},
        // {WORD, "echo", 0},
		// {WORD, "$VAR", 0},
        // {WORD, "Hello, world!", 0},
		// {WORD,"ls", 0},
		// {WORD,"-l", 0},
		// {WORD,"-a", 0},
		// {PIPE, "|", 0},
		// {WORD,"cat", 0},
		// {PIPE, "|", 0},
		// {WORD,"wc", 0},
		// {WORD,"-w", 0},
		// {WORD,"grep", 0},
		// {WORD, "grep", 0},
		// {WORD, "test", 0},
		{LESS, "<", 0},
		{WORD, "grammar_rules.txt", 0},
        {EOF_TOKEN, NULL, 0}
    };
    return tokens;
}

void print_ast(t_ast_node *node, int level)
{
	if (node == NULL) return;
	for (int i = 0; i < level; ++i) printf("  ");
	printf("%d: %s\n", node->type, node->data ? node->data : "NULL");
	printf("fd: %d\n", node->fd);
	if (node->child)
		print_ast(node->child, level + 2);
	if (node->sibling)
		print_ast(node->sibling, level);
}


int main() {
    t_token *tokens = create_mock_tokens();
    t_parser_state state;
    init_parser_state(&state, tokens);

    t_ast_node *ast = parse_complete_command(&state);
    print_ast(ast, 0);


	destroy_ast(ast);
    return 0;
}
