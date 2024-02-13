#include "minishell/execution/executor.h"
#include "minishell/execution/builtins.h"
#include "minishell/minishell.h"
#include "minishell/error.h"

int	exec_cmd(t_minishell *ms, char *cmd, char **options);
void	init_fds(t_minishell *ms);
// TODO: change arguments to be the expected structs

bool	executable_node(t_ast_node *node)
{
	return (node->type == N_CMD_WORD || node->type == N_INFILE || node->type == N_OUTFILE);
}

int	count_cmds(t_minishell *ms, t_ast_node *node, bool is_child)
{
	if (node == NULL) return (0);
	if (is_child && executable_node(node))
	{
		return (1);
	}
	return (count_cmds(ms, node->child, true) + count_cmds(ms, node->sibling, false));
}

typedef struct {
    t_ast_node *node;
    bool is_child;
} StackData;

typedef struct StackNode {
    StackData data;
    struct StackNode *next;
} StackNode;

StackNode* createStackNode(t_ast_node *node, bool is_child) {
    StackNode *newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        printf("Memory error\n");
        return NULL;
    }
    newNode->data.node = node;
    newNode->data.is_child = is_child;
    newNode->next = NULL;
    return newNode;
}

void push(StackNode **top, t_ast_node *node, bool is_child) {
    StackNode *newNode = createStackNode(node, is_child);
    if (!newNode) return;
    newNode->next = *top;
    *top = newNode;
}

StackData pop(StackNode **top) {
    if (*top == NULL) {
        printf("Stack underflow\n");
        StackData emptyData = {NULL, false};
        return emptyData;
    }
    StackNode *temp = *top;
    StackData poppedData = temp->data;
    *top = (*top)->next;
    free(temp);
    return poppedData;
}

StackData stackTop(StackNode *top) {
    if (top == NULL) {
        printf("Stack is empty\n");
        StackData emptyData = {NULL, false};
        return emptyData;
    }
    return top->data;
}

bool isEmpty(StackNode *top) {
    return top == NULL;
}

char	**siblings_to_array(t_ast_node *node)
{
	int		i;
	char	**arr;
	t_ast_node *sibling;

	i = 0;
	sibling = node->sibling;
	while (sibling)
	{
		i++;
		sibling = sibling->sibling;
	}
	arr = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	sibling = node->sibling;
	while (sibling)
	{
		arr[i] = ft_strdup(sibling->data);
		i++;
		sibling = sibling->sibling;
	}
	arr[i] = NULL;
	return (arr);
}

t_ast_node	*get_last_sibiling(t_ast_node *node)
{
	t_ast_node *sibling;

	sibling = node;
	while (sibling->sibling)
		sibling = sibling->sibling;
	return (sibling);
}

char	**get_arr_without_last(char **arr)
{
	int		i;
	char	**new_arr;

	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
		i++;
	new_arr = (char **)malloc(sizeof(char *) * i);
	i = 0;
	while (arr[i + 1])
	{
		new_arr[i] = ft_strdup(arr[i]);
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}
void execute_ast(t_minishell *ms, t_ast_node *root) {
	int	order;
	int count = count_cmds(ms, root, false);
	printf("count: %d\n", count);
	ms->last_pid = 1;
	char	**siblings;
	t_ast_node	*file_node;
	init_fds(ms);
    StackNode *stack = NULL;
	file_node = NULL;
	order = 0;
    push(&stack, root, false);

    while (!isEmpty(stack)) {
        StackData top = pop(&stack);
        t_ast_node *node = top.node;
        bool is_child = top.is_child;

        if (node == NULL) continue;

        if (is_child && executable_node(node))
		{
			if (order == 0 && node->type == N_INFILE)
			{
				file_node = node;
				node = node->sibling;
			}
			siblings = siblings_to_array(node);
            printf("data: %s\n", file_node? file_node->data : node->data);
            t_ast_node *sibling = node;
            while (sibling) {
                printf("sibling data: %s\n", sibling->data);
                sibling = sibling->sibling;
            }
			do_input_redirection(ms, !order, file_node);
			pipe(ms->pipe_fd);
			if (is_builtin(node->data) && runs_on_parent(node->data))
			{
				ms->exit_code = exec_builtin(ms, node->data, siblings);
				continue ;
			}
			ms->last_pid = fork();
			if (ms->last_pid == 0)
			{
				file_node = NULL;
				if (order + 1 == count && get_last_sibiling(node)->type == N_INFILE)
				{
					file_node = get_last_sibiling(node);
					siblings = get_arr_without_last(siblings);
					int i = 0;
					while (siblings[i])
					{
						printf("siblings: %s\n", siblings[i]);
						i++;
					}
				}
				do_output_redirection(ms, order + 1 == count, file_node);
				use_child_signals();
				exec_cmd(ms, node->data, siblings);
			}
			order++;
        }

        if (node->sibling) {
            push(&stack, node->sibling, false);
        }

        if (node->child) {
            push(&stack, node->child, true);
        }
    }
	restore_io(ms->system_fd, ms->pipe_fd, count == 0);
	wait_for_children(ms->last_pid, ms);
}

void	executor(t_minishell *ms, int order)
{
	int		i;
	(void)order;
	i = 0;
	ms->last_pid = 1;
	
	init_fds(ms);
	while (ms->cmds[i])
	{
		// do_input_redirection(ms->pipe_fd, !i, ms->in_fd);
		pipe(ms->pipe_fd);
		if (is_builtin(ms->cmds[i]) && runs_on_parent(ms->cmds[i]))
		{
			ms->cmds[i] = trim_end(trim_start(ms->cmds[i], true), true);
			ms->exit_code = exec_builtin(ms, ms->cmds[i], ms->cmds);
			i++;
			continue ;
		}
		ms->last_pid = fork();
		if (ms->last_pid == 0)
		{
			// do_output_redirection(ms->pipe_fd, !ms->cmds[i + 1], ms->system_fd[1], ms->out_fd);
			use_child_signals();
			ms->cmds[i] = trim_end(trim_start(ms->cmds[i], true), true);
			exec_cmd(ms, ms->cmds[i], ms->cmds);
		}
		i++;
	}
	// restore_io(ms->system_fd, ms->pipe_fd);
	wait_for_children(ms->last_pid, ms);
}

char	**join_cmd_and_options(char *cmd, char **options)
{
	char	**cmd_and_options;
	int		i;

	i = 0;
	while (options[i])
		i++;
	cmd_and_options = (char **)malloc(sizeof(char *) * (i + 2));
	cmd_and_options[0] = ft_strdup(cmd);
	i = 0;
	while (options[i])
	{
		cmd_and_options[i + 1] = ft_strdup(options[i]);
		i++;
	}
	cmd_and_options[i + 1] = NULL;
	return (cmd_and_options);
}

// TODO: change arguments to be the expected structs
int	exec_cmd(t_minishell *ms, char *cmd, char **options)
{
	char	*path;
	char	**parts;

	if (is_builtin(cmd))
	{
		exec_builtin(ms, cmd, options);
		// TODO: free allocated memory
		exit(ms->exit_code);
	}
	parts = join_cmd_and_options(cmd, options);
	path = get_path(cmd, ms->envp);
	if (execve(path, parts, ms->envp) == -1)
	{
		str_arr_free(parts);
		if (!access(path, F_OK) && access(path, X_OK) < 0)
			err(cmd, "Permission denied", 126, ms);
		err(cmd, "command not found", 127, ms);
	}
	return (EXIT_SUCCESS);
}

void	init_fds(t_minishell *ms)
{
	ms->system_fd[0] = dup(STDIN_FILENO);
	ms->system_fd[1] = dup(STDOUT_FILENO);
	ms->pipe_fd[0] = 0;
	ms->pipe_fd[1] = 0;
}