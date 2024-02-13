/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 23:46:26 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/14 00:03:25 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/stack.h"

t_stack_node	*create_stack_node(t_ast_node *node, bool is_child)
{
    t_stack_node *new_node = (t_stack_node*)malloc(sizeof(t_stack_node));
    if (!new_node)
        return (NULL);
    new_node->data.node = node;
    new_node->data.is_child = is_child;
    new_node->next = NULL;
    return (new_node);
}

void stack_push(t_stack_node **top, t_ast_node *node, bool is_child) {
    t_stack_node *new_node = create_stack_node(node, is_child);
    if (!new_node) return;
    new_node->next = *top;
    *top = new_node;
}

t_stack_data	stack_pop(t_stack_node **top)
{
    if (*top == NULL) {
        printf("Stack underflow\n");
        t_stack_data empty_data = {NULL, false};
        return empty_data;
    }
    t_stack_node *temp = *top;
    t_stack_data popped_data = temp->data;
    *top = (*top)->next;
    free(temp);
    return (popped_data);
}

t_stack_data	stack_top(t_stack_node *top)
{
    if (top == NULL) {
        printf("Stack is empty\n");
        t_stack_data empty_data = {NULL, false};
        return (empty_data);
    }
    return (top->data);
}

bool stack_is_empty(t_stack_node *top) {
    return top == NULL;
}
