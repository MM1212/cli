/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:54:25 by martiper          #+#    #+#             */
/*   Updated: 2024/03/21 12:56:43 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "option/utils.h"
#include <option/flag.h>
#include <libft.h>

void	cli_cleanup_option(t_cli_option *option)
{
	if (option->slug)
		free(option->slug);
	if (option->description)
		free(option->description);
	if (option->default_value)
		free(option->default_value);
	if (option->choices)
		ft_split_free(option->choices);
	if (option->value)
		free(option->value);
	for (uint32_t i = 0; i < option->_switches_size; i++)
		cli_cleanup_flag(&option->_switches[i]);
	free(option->_switches);
	if (option->_flags)
		free(option->_flags);
}
