/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:59:01 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/10 19:49:03 by ramoussa         ###   ########.fr       */
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

int builtins_pwd();
int builtins_env(char **envp);
void builtins_exit(t_minishell *ms);
void builtins_echo(char **cmds);
int builtins_export(char **args, t_minishell *ms);
int builtins_unset(char **args, t_minishell *ms);
int	builtins_cd(char **cmds, char ***envp);

int is_builtin(char *cmd);
int runs_on_parent(char *cmd);
int exec_builtin(t_minishell *ms, char *cmd);

#endif