#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"

int builtins_echo(char **cmds)
{
    int i = 1;
    int newline = 1;

    if (cmds[1] && !ft_strncmp(cmds[1], "-n", 2))
    {
        newline = 0;
        i++;
    }
    while (cmds[i])
    {
        printf("%s", cmds[i]);
        if (cmds[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (EXIT_SUCCESS);
}
