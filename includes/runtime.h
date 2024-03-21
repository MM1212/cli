/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:42:51 by martiper          #+#    #+#             */
/*   Updated: 2024/03/21 16:44:54 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNTIME_H
# define RUNTIME_H

#include <defs.h>
#include <stddef.h>

typedef struct s_cli_runtime	t_cli_runtime;

struct s_cli_runtime
{
	int		argc;
	char	**argv;
	bool	on_switch;
	bool	on_flag;
};

# endif
