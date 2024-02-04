/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:59:01 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/04 13:06:11 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include "baselib.h"

int builtins_pwd();
int builtins_env(char **envp);
int builtins_exit(char **cmds, char **envp);
int builtins_echo(char **cmds);
int builtins_export(char **args, char ***envp);
int builtins_unset(char **args, char ***envp);

int is_builtin(char *cmd);
int runs_on_parent(char *cmd);
int exec_builtin(char **cmds, char *cmd, char ***envp);

#endif