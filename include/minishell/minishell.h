/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:37:16 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/14 19:37:20 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <unistd.h>
# include <stdbool.h>
# include "baselib.h"
# include <stdlib.h>
# include "minishell/stack.h"

extern struct termios	settings;

typedef struct	s_minishell {
	char	**envp;
	char	**args;
	char	*input;
	int		pipe_fd[2];
	int		system_fd[2];
	int		exit_code;
	pid_t	last_pid;
	t_ast_node	*ast;
	t_ast_node	*file_node;
	char		*first_cmd;
	t_stack_node	*stack;
	int			count;
}	t_minishell;

void	update_terminos(void);
void    reset_terminos(void);
void    use_parent_signals(void);
void    use_child_signals(void);

int     exists_in_env(char *key, char **envp);
int     update_env_variable(char *key, char *value, t_minishell *ms);
char    **add_to_env(char *key, char *value, char **envp);
char    **remove_from_env(char *key, char **envp);
int     is_valid_env_name(char *name);
char 	*get_env_value(char *key, char **envp);
char 	*get_env_key(char *key);
char	**copy_env(char **envp);
int		free_env(char **envp);
char	**get_environment(void);

void	expand_ast(t_minishell *ms, t_ast_node *node, int level);
char	*expand(t_minishell *ms, char *cmds);
char	*trim_start(char *str, bool free_str);
char	*trim_end(char *str, bool free_str);

#endif