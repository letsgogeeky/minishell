//TODO: add libft funcs
// #include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum ast_node_type_s
{
	COMMAND,
	PIPE,
	REDIRECTION_IN,
	REDIRECTION_OUT,
	REDIRECTION_WORD
}	ast_node_type_t;

typedef struct ast_node_s
{
	ast_node_type_t		type;
	struct ast_node_s	*parent;
	struct ast_node_s	*first_child;
	struct ast_node_s	*next_sibling;
	struct ast_node_s	*last_child;
	char				*data;
	char				*filename;
	size_t				data_len;			
}	ast_node_t;


ast_node_t *ast_new_node(ast_node_type_t type, char *value)
{
	ast_node_t *node;

	node = malloc(sizeof(ast_node_t));
	if (!node)
		return (NULL);

	node->type = type;
	node->parent = NULL;
	node->first_child = NULL;
	node->next_sibling = NULL;
	node->last_child = NULL;
	node->data = strdup(value);
	if (!node->data)
		return(free(node), NULL);
	node->filename = NULL;
	node->data_len = strlen(value);
	return (node);
}

