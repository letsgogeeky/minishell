/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:34:53 by ramymoussa        #+#    #+#             */
/*   Updated: 2024/02/13 15:32:41 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/execution/executor.h"
#include "minishell/minishell.h"

void	do_input_redirection(t_minishell *ms, bool is_first_cmd, t_ast_node *file)
{
    //  TODO: update to check if input is stdin or file
    if (is_first_cmd && file->fd != -1)
    {
        dup2(file->fd, STDIN_FILENO);
        close(file->fd);
    }
    else
        dup2(ms->pipe_fd[0], STDIN_FILENO);
    if (!is_first_cmd)
    {
        close(ms->pipe_fd[0]);
        close(ms->pipe_fd[1]);
    }
}

void	do_output_redirection(t_minishell *ms, bool is_last_cmd, t_ast_node *file)
{
    if (is_last_cmd && file->fd != -1)
    {
        ft_putendl_fd("here", STDERR_FILENO);
        dup2(file->fd, STDOUT_FILENO);
        close(file->fd);
    }
    else if (is_last_cmd)
        dup2(ms->system_fd[1], STDOUT_FILENO);
    else
        dup2(ms->pipe_fd[1], STDOUT_FILENO);
    close(ms->pipe_fd[1]);
    close(ms->pipe_fd[0]);
    close(ms->system_fd[0]);
}

void restore_io(int *system_io, int *pipe_io, bool is_empty)
{
    dup2(system_io[0], STDIN_FILENO);
    dup2(system_io[1], STDOUT_FILENO);
    close(system_io[0]);
    close(system_io[1]);
	if (is_empty)
		return ;
    close(pipe_io[1]);
    close(pipe_io[0]);
}

int parse_heredoc(char *demlimiter, char **envp)
{
    char	*doc;
	int		ipc[2];

	(void)envp;
	if (pipe(ipc))
		printf("pipe error\n");
    doc = NULL;
	if (isatty(STDIN_FILENO))
		doc = readline("heredoc> "); // this is a MacOS style of prompt, if you want normal shell style use "> "
	while (doc && ft_strncmp(demlimiter, doc, ft_strlen(doc)))
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
	return (0);
}
