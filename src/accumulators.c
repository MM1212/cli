/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accumulators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:23:45 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 11:09:03 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__accumulators.h"
#include <defs.h>

static t_cli_option_builder	g_opt_builder;
static t_cli_handle			g_handle;

t_cli_option_builder*	g_cli_opt_builder = &g_opt_builder;
t_cli_handle*			g_cli_handle = &g_handle;

t_cli_option_builder	*cli_acc_get_opt_builder(void)
{
	return (g_cli_opt_builder);
}

t_cli_handle			*cli_acc_get_handle(void)
{
	return (g_cli_handle);
}
