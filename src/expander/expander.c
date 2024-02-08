#include "minishell/minishell.h"

char	*expand_variable(char *cursor, int j, char **envp);
int	key_length(char *start);

void expand(char ***cmds_lst, char ***envp)
{
    int i;
    int j;
    char *expanded;
    char *tmp;
    char **cmds;

    i = -1;
    cmds = *cmds_lst;
    while (cmds[++i])
    {
        j = -1;
        while (cmds[i][++j])
        {
            if (cmds[i][j] == '$')
            {
				expanded = expand_variable(cmds[i], j, *envp);
                tmp = expanded;
                expanded = ft_strjoin(expanded, cmds[i] + j + 1 + key_length(cmds[i] + j + 1));
                free(tmp);
                free(cmds[i]);
                cmds[i] = expanded;
            }
        }
    }
}

int	key_length(char *start)
{
	char *key;
	int len;

	key = get_env_key(start);
	len = ft_strlen(key);
	free(key);
	return (len);
}

char	*expand_variable(char *cursor, int j, char **envp)
{
	char *expanded;
	char *tmp;
	char *value;

	tmp = ft_substr(cursor, 0, j);
	if (cursor[j+1] && cursor[j+1] == '?')
		expanded = ft_strjoin(tmp, ft_itoa(g_exit_code));
	else
	{
		value = get_env_value(cursor + j + 1, envp);
		expanded = ft_strjoin(tmp, value);
		free(value);
	}
	free(tmp);
	return (expanded);
}
