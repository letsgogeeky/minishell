/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 03:03:03 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/14 03:03:39 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

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
