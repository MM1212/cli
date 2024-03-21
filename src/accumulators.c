/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accumulators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:23:45 by martiper          #+#    #+#             */
/*   Updated: 2024/03/21 12:28:25 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__accumulators.h"
#include <defs.h>

t_cli_option_builder	*cli_acc_get_opt_builder(void)
{
	static t_cli_option_builder	opt_builder;

	return (&opt_builder);
}

t_cli_handle			*cli_acc_get_handle(void)
{
	static t_cli_handle	handle;

	return (&handle);
}
