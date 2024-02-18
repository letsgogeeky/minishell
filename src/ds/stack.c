/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 23:46:26 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 21:34:30 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

t_stack_node	*create_stack_node(t_ast_node *node, bool is_child)
{
	t_stack_node	*new_node;

	new_node = (t_stack_node *) malloc(sizeof (t_stack_node));
	if (!new_node)
		return (NULL);
	new_node->data.node = node;
	new_node->data.is_child = is_child;
	new_node->next = NULL;
	return (new_node);
}

void	stack_push(t_stack_node **top, t_ast_node *node, bool is_child)
{
	t_stack_node	*new_node;

	new_node = create_stack_node(node, is_child);
	if (!new_node)
		return ;
	new_node->next = *top;
	*top = new_node;
}

t_stack_data	stack_pop(t_stack_node **top)
{
	t_stack_node	*temp;
	t_stack_data	popped_data;
	t_stack_data	empty_data;

	if (*top == NULL)
	{
		printf("Stack underflow\n");
		empty_data.node = NULL;
		empty_data.is_child = false;
		return (empty_data);
	}
	temp = *top;
	popped_data = temp->data;
	*top = (*top)->next;
	free(temp);
	return (popped_data);
}

t_stack_data	stack_top(t_stack_node *top)
{
	t_stack_data	empty_data;

	if (top == NULL)
	{
		printf("Stack is empty\n");
		empty_data.node = NULL;
		empty_data.is_child = false;
		return (empty_data);
	}
	return (top->data);
}

bool	stack_is_empty(t_stack_node *top)
{
	return (top == NULL);
}
