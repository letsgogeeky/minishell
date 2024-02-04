
#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdio.h>
# include <stdbool.h>
# include "baselib.h"

struct s_executor
{
	pid_t	last_pid;
	char	**envp;
	int		pipe_in_fd;
	int		pipe_out_fd;
};

char	*get_path(char *cmd, char **envp);
void	do_input_redirection(int *pipe_io, bool is_first_cmd, int file_fd);
void	do_output_redirection(int *pipe_io, bool is_last_cmd, int system_out_fd, int file_fd);
void	restore_io(int *system_io, int *pipe_io);
void	executor(char **cmds, char ***envp, int out_fd, int in_fd);

#endif