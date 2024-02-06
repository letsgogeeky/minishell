#include "minishell/minishell.h"
#include "minishell/execution/builtins.h"
#include "minishell/error.h"

int builtins_export(char **args, char ***envp)
{
    int i;
    int j;
    char *key;
    char *value;

    if (!envp)
        return (print_builtin_error("export", NULL, "no envp"), EXIT_FAILURE);
    i = 0;
    while (args[i])
    {
        j = 0;
        while (args[i][j] && args[i][j] != '=')
            j++;
        key = ft_substr(args[i], 0, j);
        printf("key: %s\n", key);
        if (!is_valid_env_name(key))
            return (free(key),
            print_builtin_error("export", key, "not a valid identifier"), 
            EXIT_FAILURE);
        value = ft_substr(args[i], j + 1, ft_strlen(args[i]) - j - 1);
		printf("exists_in_env(key, *envp): %d\n", exists_in_env(key, *envp));
        if (exists_in_env(key, *envp) == -1)
            *envp = add_to_env(key, value, *envp);
        else
            update_env_variable(key, value, envp);
        free(key);
        free(value);
        i++;
    }
    return (EXIT_SUCCESS);
}
