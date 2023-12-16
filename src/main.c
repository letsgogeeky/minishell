/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 21:12:46 by ramymoussa        #+#    #+#             */
/*   Updated: 2023/12/16 19:01:09 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"

static int interactive_mode()
{
    char    *line;

    line = readline("massiveshell$ ");
    while (line)
    {
        if (line && line[0])
        {
            // TODO: add to history and do execution magic and return exit code after
            builtins_pwd();
        }
        line = readline("massiveshell$ ");
    }
    return (0);
}

int main(int argc, char **argv)
{
    (void)argv;
    if (argc >= 2)
        return (1); // TODO: handle non-interactive mode if required or desired
    if (isatty(STDIN_FILENO))
    {
        // TODO: Start an interactive shell and do magic
        interactive_mode();
    }
    return (0);
}