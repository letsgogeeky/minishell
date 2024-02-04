#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <unistd.h>
# include <stdbool.h>
# include "baselib.h"

void	update_terminos(void);
void    reset_terminos(void);
void    use_parent_signals();
void    use_child_signals();
int     open_file(char *path, int flags);

int     exists_in_env(char *key, char **envp);
int     update_env_variable(char *key, char *value, char ***envp);
char    **add_to_env(char *key, char *value, char **envp);
char    **remove_from_env(char *key, char **envp);
int     is_valid_env_name(char *name);

#endif