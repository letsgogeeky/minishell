/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:59:01 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/13 15:44:50 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell/minishell.h"
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include "baselib.h"

int builtins_pwd(void);
int builtins_env(char **envp);
void builtins_exit(t_minishell *ms);
void builtins_echo(char **cmds);
int builtins_export(char **args, t_minishell *ms);
int builtins_unset(char **args, t_minishell *ms);
int	builtins_cd(char **cmds, t_minishell *ms);
int is_builtin(char *cmd);
int runs_on_parent(char *cmd);
int exec_builtin(t_minishell *ms, char *cmd, char **options);

#endif