/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 21:25:10 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/03 20:10:03 by ramymoussa       ###   ########.fr       */
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

void	update_terminos(void);
void    reset_terminos(void);
void    use_parent_signals();
void    use_child_signals();

int     exists_in_env(char *key, char **envp);
int     update_env_variable(char *key, char *value, char ***envp);
char    **add_to_env(char *key, char *value, char **envp);

#endif