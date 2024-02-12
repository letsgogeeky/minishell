#include "minishell/minishell.h"

char *get_env_value(char *key, char **envp)
{
    int i;
    int j;
    char *value;
	printf("key: %s\n", key);
    i = 0;
    while (envp[i])
    {
        j = 0;
        while (envp[i][j] && envp[i][j] == key[j])
            j++;
        if (envp[i][j] == '=' && (!key[j] || key[j] == ' '))
        {
            value = ft_strdup(envp[i] + j + 1);
            return (value);
        }
        i++;
    }
    return (ft_strdup(""));
}

int is_valid_env_key_char(char c) {
    return c && c != ' ' && \
		c != '$' && c != '=' && \
		c != '/' && c != '\\' && \
		c != '\'' && c != '\"' && \
		c != ';' && c != '|' && c != '&' && \
		c != '(' && c != ')' && \
		c != '{' && c != '}' && c != '<' && c != '>';
}

char *get_env_key(char *key)
{
    int i;

    i = 0;
    while (key[i] && is_valid_env_key_char(key[i]))
        i++;
    return (ft_substr(key, 0, i));
}

char	**get_environment(void)
{
	extern char	**environ;
	char		**envp;
	int			i;

	i = 0;
	while (environ[i])
		i++;
	envp = malloc(sizeof(char *) * (i + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (environ[i])
	{
		envp[i] = ft_strdup(environ[i]);
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

char	**copy_env(char **envp)
{
	char	**copy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

int	free_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	return (EXIT_SUCCESS);
}
