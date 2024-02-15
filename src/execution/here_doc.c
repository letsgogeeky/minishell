/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 03:03:03 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/15 01:15:11 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

char	*remove_trailing_newline(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (str[len - 1] == '\n')
		str[len - 1] = '\0';
	return (str);
}

int parse_heredoc(t_minishell *ms, t_ast_node *node)
{
    char	*doc;
	int		fd;
	char	*delimiter;

	delimiter = node->data;
	fd = open("/tmp/demons", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    doc = NULL;
	if (isatty(STDIN_FILENO))
		doc = readline("heredoc> "); // this is a MacOS style of prompt, if you want normal shell style use "> "
	doc = remove_trailing_newline(doc);
	while (doc && ft_strncmp(delimiter, doc, ft_strlen(doc)))
	{
		doc = expand(ms, doc);
		ft_putstr_fd(doc, fd);
		ft_putstr_fd("\n", fd);
		free(doc);
		doc = readline("heredoc> ");
	}
	if (doc)
		free(doc);
	close(fd);
	node->fd = open("/tmp/demons", O_RDONLY);
	node->data = ft_strdup("/tmp/demons");
	return (0);
}



