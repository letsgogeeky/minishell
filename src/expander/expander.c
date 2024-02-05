#include "minishell/minishell.h"

char *get_env_value(char *key, char **envp);
char *get_env_key(char *key);

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

char *get_env_value(char *key, char **envp)
{
    int i;
    int j;
    char *value;

    i = 0;
    while (envp[i])
    {
        j = 0;
        while (envp[i][j] && envp[i][j] == key[j])
            j++;
        if (envp[i][j] == '=' && !key[j])
        {
            value = ft_strdup(envp[i] + j + 1);
            return (value);
        }
        i++;
    }
    return (ft_strdup(""));
}

char *get_env_key(char *key)
{
    int i;

    i = 0;
    while (key[i] && key[i] != '=')
        i++;
    return (ft_substr(key, 0, i));
}
