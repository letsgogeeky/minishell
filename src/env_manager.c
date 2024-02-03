#include "minishell/minishell.h"

int    exists_in_env(char *key, char **envp)
{
    int i;
    int j;

    i = 0;
    while (envp[i])
    {
        j = 0;
        while (envp[i][j] && envp[i][j] == key[j])
            j++;
        if (envp[i][j] == '=' && !key[j])
            return (i);
        i++;
    }
    return (-1);
}

int update_env_variable(char *key, char *value, char ***envp)
{
    int i;
    int j;
    char **env;

    env = *envp;
    i = exists_in_env(key, env);
    j = 0;
    if (i == -1)
        return (-1);
    if (env[i][j] == '=' && !key[j])
    {
        free(env[i]);
        env[i] = ft_strjoin(key, value);
        return (0);
    }
    return (-1);
}

char    **add_to_env(char *key, char *value, char **envp)
{
    int i;
    char **new_envp;

    i = 0;
    while (envp[i])
        i++;
    new_envp = malloc(sizeof(char *) * (i + 2));
    i = 0;
    while (envp[i])
    {
        new_envp[i] = envp[i];
        i++;
    }
    key = ft_strjoin(key, "=");
    new_envp[i] = ft_strjoin(key, value);
    new_envp[i + 1] = NULL;
    return (new_envp);
}
