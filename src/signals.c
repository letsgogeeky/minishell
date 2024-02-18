/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 00:26:30 by ramoussa          #+#    #+#             */
/*   Updated: 2024/02/18 21:02:11 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

void	handle_sigint(int sig)
{
	if (sig != SIGINT)
		return ;
	write(STDIN_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	update_terminos(void)
{
	tcgetattr(STDIN_FILENO, &g_settings);
	g_settings.c_lflag &= ~(ECHOCTL | IEXTEN);
	tcsetattr(STDIN_FILENO, TCSANOW, &g_settings);
}

void	reset_terminos(void)
{
	tcgetattr(STDIN_FILENO, &g_settings);
	g_settings.c_lflag |= (ECHOCTL | IEXTEN);
	tcsetattr(STDIN_FILENO, TCSANOW, &g_settings);
}

void	use_parent_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handle_sigint);
	update_terminos();
}

void	use_child_signals(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}
