#include "minishell/minishell.h"

int expand(char ***cmds_lst, char ***envp)
{
    int i;
    int j;
    char *expanded;
    char *tmp;
    char **cmds;

    i = 0;
    cmds = *cmds_lst;
    while (cmds[i])
    {
        j = 0;
        while (cmds[i][j])
        {
            if (cmds[i][j] == '$')
            {
                tmp = ft_substr(cmds[i], 0, j);
                if (cmds[i][j+1] && cmds[i][j+1] == '?')
                    expanded = ft_strjoin(tmp, ft_itoa(g_exit_code));
                else
                    expanded = ft_strjoin(tmp, get_env_value(cmds[i] + j + 1, *envp));
                free(tmp);
                tmp = expanded;
                expanded = ft_strjoin(expanded, cmds[i] + j + 1 + ft_strlen(get_env_key(cmds[i] + j + 1)));
                free(tmp);
                free(cmds[i]);
                cmds[i] = expanded;
            }
            j++;
        }
        i++;
    }
    return (EXIT_SUCCESS);
}
