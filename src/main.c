/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 21:12:46 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/01/13 19:14:18 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/execution/executor.h"

static int interactive_mode(char **envp)
{
    char    *line;
    char    **cmds;

    line = readline("massiveshell$ ");
    while (line && line[0] != EOF)
    {
        use_parent_signals();
        if (line && line[0])
        {
            // TODO: add to history and do execution magic and return exit code after
            // builtins_pwd();
            cmds = ft_split(line, '|');
			executor(cmds, envp); // TODO: make this proper executor
        }
        line = readline("massiveshell$ ");
    }
    printf("CTRL+D\n");
    // This function should also be called when exiting the shell using `exit` builtin
    reset_terminos();
    return (0);
}

int main(int argc, char **argv, char **envp)
{
    (void)argv;
    if (argc >= 2)
        return (1); // TODO: handle non-interactive mode if required or desired
    if (isatty(STDIN_FILENO))
    {
        use_parent_signals();
        // TODO: Start an interactive shell and do magic
        interactive_mode(envp);
    }
    return (0);
}