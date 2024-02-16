/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 03:03:03 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/16 03:41:52 by ramoussa         ###   ########.fr       */
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

char	*get_readline_title(char *delimiter)
{
	if (delimiter[0] == '\'')
		return (ft_strdup("quote> "));
	return (ft_strdup("dquote> "));
}

char	*parse_unclosed_quote(t_minishell *ms, char *delimiter, char *base)
{
	char	*doc;
	char	*tmp;
	char	*title;

	doc = ft_substr(base, 1, ft_strlen(base));
	doc = ft_strjoin_s1_free(doc, "\n");
	title = get_readline_title(delimiter);
	tmp = readline(title);
	tmp = remove_trailing_newline(tmp);
	while (tmp && ft_strncmp(delimiter, tmp, ft_strlen(tmp)))
	{
		if (delimiter[0] != '\'')
			tmp = expand(ms, tmp);
		doc = ft_strjoin_s1_free(doc, tmp);
		doc = ft_strjoin_s1_free(doc, "\n");
		free(tmp);
		tmp = readline(title);
	}
	free(tmp);
	free(title);
	return (doc);
}
