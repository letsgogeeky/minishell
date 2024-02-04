/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:34:53 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/03 15:28:10 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"
#include "minishell/minishell.h"

void	do_input_redirection(int *pipe_io, bool is_first_cmd, int file_fd)
{
    //  TODO: update to check if input is stdin or file
    if (is_first_cmd && file_fd != -1)
    {
        dup2(file_fd, STDIN_FILENO);
        close(file_fd);
    }
    else
        dup2(pipe_io[0], STDIN_FILENO);
    if (!is_first_cmd)
    {
        close(pipe_io[0]);
        close(pipe_io[1]);
    }
}

void	do_output_redirection(int *pipe_io, bool is_last_cmd, int system_out_fd, int file_fd)
{
    if (is_last_cmd && file_fd != -1)
    {
        // ft_putendl_fd("here", STDERR_FILENO);
        dup2(file_fd, STDOUT_FILENO);
        close(file_fd);
    }
    else if (is_last_cmd)
        dup2(system_out_fd, STDOUT_FILENO);
    else
        dup2(pipe_io[1], STDOUT_FILENO);
    close(pipe_io[1]);
    close(pipe_io[0]);
    close(system_out_fd);
}

void restore_io(int *system_io, int *pipe_io)
{
    dup2(system_io[0], STDIN_FILENO);
    dup2(system_io[1], STDOUT_FILENO);
    close(system_io[0]);
    close(system_io[1]);
    close(pipe_io[1]);
    close(pipe_io[0]);
    
}

int parse_heredoc(char *demlimiter, char **envp)
{
    char	*doc;
	char	*limiter;
	int		ipc[2];

	if (pipe(ipc))
		printf("pipe error\n");
    doc = NULL;
    if (isatty(STDIN_FILENO))
        doc = readline("heredoc> "); // this is a MacOS style of prompt, if you want normal shell style use "> "
	while (doc && ft_strncmp(limiter, doc, ft_strlen(doc)))
	{
		ft_putstr_fd(doc, ipc[1]);
		free(doc);
		doc = readline("heredoc> ");
	}
	if (doc)
		free(doc);
	dup2(ipc[0], 0);
	close(ipc[0]);
	close(ipc[1]);
}