/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexi_lex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 22:47:39 by fvoicu            #+#    #+#             */
/*   Updated: 2024/02/19 01:06:17 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/parsing/lexer.h"
#include "baselib.h"

void	update_state(t_lexer *lexer, char curr_ch)
{
	lexer->state = in_error;
	if (ra_is_whiteshapce(curr_ch))
		lexer->state = in_separator;
	else if (curr_ch == '|')
		lexer->state = in_pipe;
	else if (curr_ch == '<' && *(lexer->current + 1) != '<')
		lexer->state = in_less;
	else if (curr_ch == '>' && *(lexer->current + 1) != '>')
		lexer->state = in_great;
	else if (curr_ch == '<' && *(lexer->current + 1) == '<')
		lexer->state = in_dless;
	else if (curr_ch == '>' && *(lexer->current + 1) == '>')
		lexer->state = in_dgreat;
	else if (curr_ch == '=')
		lexer->state = in_assignment;
	else if (curr_ch == '-')
		lexer->state = in_word;
	else if (curr_ch == '"')
		lexer->state = in_dqoute;
	else if (curr_ch == '\'')
		lexer->state = in_squote;
	else if (curr_ch != '\0')
		lexer->state = in_word;
}

t_token	*get_next_token(t_lexer *lexer)
{
	while (*lexer->current)
	{
		update_state(lexer, *lexer->current);
		if (lexer->state == in_word || lexer->state == in_option)
			return (command_token(lexer));
		else if (lexer->state == in_pipe)
			return (pipe_token(lexer));
		else if (lexer->state == in_less || lexer->state == in_great)
			return (redirect_token (lexer));
		else if (lexer->state == in_dless || lexer->state == in_dgreat)
			return (dredirect_token (lexer));
		else if (lexer->state == in_assignment)
			return (assignment_token(lexer));
		else if (lexer->state == in_dqoute || lexer->state == in_squote)
			return (quoted_token(lexer));
		else if (lexer->state == in_separator)
			++lexer->current;
		else if (lexer->state == in_error)
			return (error_token(lexer));
	}
	return (new_token(EOF_TOKEN, lexer));
}

void	init_lexer(t_lexer *lexer, const char *input)
{
	lexer->input_buffer = ft_strdup(input);
	if (!lexer->input_buffer)
	{
		ft_putstr_fd("Failed to allocate memory for input buffer\n", \
					STDERR_FILENO);
		exit(1);
	}
	lexer->current = lexer->input_buffer;
	lexer->start = lexer->input_buffer;
	lexer->state = start;
}

void	destroy_lexer(t_lexer *lexer)
{
	free(lexer->input_buffer);
}

t_token	*lex(const char *input)
{
	t_lexer	lexer;
	size_t	token_capacity;
	t_token	*tokens;
	size_t	tokens_size;
	t_token	*tmp;

	token_capacity = 10;
	tokens = init_token_array(token_capacity);
	tokens_size = 0;
	init_lexer(&lexer, input);
	while (1)
	{
		if (tokens_size >= token_capacity)
			tokens = realloc_token_array(tokens, &token_capacity);
		tmp = get_next_token(&lexer);
		tokens[tokens_size] = *tmp;
		free(tmp);
		if (tokens[tokens_size].type == EOF_TOKEN)
			break ;
		++tokens_size;
	}
	destroy_lexer(&lexer);
	return (tokens);
}
