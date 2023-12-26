/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 21:12:46 by ramymoussa        #+#    #+#             */
/*   Updated: 2023/12/26 21:32:04 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/execution/executor.h"

static int interactive_mode(char **envp)
{
    char    *line;

    line = readline("massiveshell$ ");
    while (line)
    {
        if (line && line[0])
        {
            // TODO: add to history and do execution magic and return exit code after
            // builtins_pwd();
			exec_cmd(line, envp); // TODO: make this proper executor
        }
        line = readline("massiveshell$ ");
    }
    return (0);
}

int main(int argc, char **argv, char **envp)
{
    (void)argv;
    if (argc >= 2)
        return (1); // TODO: handle non-interactive mode if required or desired
    if (isatty(STDIN_FILENO))
    {
        // TODO: Start an interactive shell and do magic
        interactive_mode(envp);
    }
    return (0);
}