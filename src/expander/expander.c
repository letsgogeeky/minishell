#include "minishell/minishell.h"

char	*expand_variable(char *cursor, int j, char **envp, int exit_code);
int	key_length(char *start);

void expand(t_minishell *ms)
{
    int i;
    int j;
    char *expanded;
    char *tmp;
	bool	is_quoted;

	is_quoted = false;
    i = -1;
    while (ms->cmds[++i])
    {
        j = -1;
        while (ms->cmds[i][++j])
        {
			if (ms->cmds[i][j] == '\'')
				is_quoted = !is_quoted;
            if (ms->cmds[i][j] == '$' && !is_quoted)
            {
				expanded = expand_variable(ms->cmds[i], j, ms->envp, ms->exit_code);
                tmp = expanded;
                expanded = ft_strjoin(expanded, ms->cmds[i] + j + 1 + key_length(ms->cmds[i] + j + 1));
                free(tmp);
                free(ms->cmds[i]);
                ms->cmds[i] = expanded;
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

char	*expand_variable(char *cursor, int j, char **envp, int exit_code)
{
	char *expanded;
	char *tmp;
	char *value;
	char *key;

	tmp = ft_substr(cursor, 0, j);
	if (cursor[j+1] && cursor[j+1] == '?')
		expanded = ft_strjoin(tmp, ft_itoa(exit_code));
	else
	{
		key = get_env_key(cursor + j + 1);
		value = get_env_value(key, envp);
		expanded = ft_strjoin(tmp, value);
		free(value);
		free(key);
	}
	free(tmp);
	return (expanded);
}

bool	is_between_single_quotes(char *str, int i)
{
	int count;

	count = 0;
	while (--i >= 0)
	{
		if (str[i] == '\'')
			count++;
	}
	return (count % 2);
}
