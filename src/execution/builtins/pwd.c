/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramymoussa <ramymoussa@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:38:37 by ramymoussa        #+#    #+#             */
/*   Updated: 2023/12/16 19:07:10 by ramymoussa       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

//TODO: Remove and use a libft function
#include <stdio.h>

#include "minishell/error.h"
#include "minishell/execution/builtins.h"

int builtins_pwd()
{
    char    path[PATH_MAX];

    if (!getcwd(path, PATH_MAX))
    {
        print_builtin_error(NULL, NULL, NULL);
        return (EXIT_FAILURE);
    }
    printf("%s\n", path);
    return (EXIT_SUCCESS);
}
