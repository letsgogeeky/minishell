/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramoussa <ramoussa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:38:37 by ramymoussa        #+#    #+#             */
/*   Updated: 2023/12/29 21:38:49 by ramoussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "minishell/error.h"
#include "minishell/execution/builtins.h"

int builtins_pwd()
{
    char    path[PATH_MAX];

    // To test failure it, change array size to be of a small number like 4, should result in a failure.
    if (!getcwd(path, PATH_MAX))
    {
        print_builtin_error("pwd", NULL, strerror(errno));
        return (EXIT_FAILURE);
    }
    printf("%s\n", path);
    return (EXIT_SUCCESS);
}
