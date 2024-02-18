/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 23:48:08 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 21:08:01 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_H
# define STACK_H

# include <stdbool.h>
# include "minishell/parsing/parser.h"

typedef struct s_stack_data
{
	t_ast_node			*node;
	bool				is_child;
}	t_stack_data;

typedef struct s_stack_node
{
	t_stack_data		data;
	struct s_stack_node	*next;
}	t_stack_node;

t_stack_node	*create_stack_node(t_ast_node *node, bool is_child);
void			stack_push(t_stack_node **head, \
						t_ast_node *node, bool is_child);
t_stack_data	stack_pop(t_stack_node **head);
t_stack_data	stack_top(t_stack_node *top);
bool			stack_is_empty(t_stack_node *head);

#endif