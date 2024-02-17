/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:59:01 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/17 23:44:36 by ramoussa         ###   ########.fr       */
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

int		builtins_pwd(void);
int		builtins_env(char **envp);
void	builtins_exit(t_minishell *ms, char **options, char *cmd);
void	builtins_echo(t_minishell *ms, char **cmds);
int		builtins_export(char **args, t_minishell *ms);
int		builtins_unset(char **args, t_minishell *ms);
int		builtins_cd(char **cmds, t_minishell *ms);
int		is_builtin(char *cmd);
int		runs_on_parent(char *cmd);
int		exec_builtin(t_minishell *ms, char *cmd, char **options);

#endif