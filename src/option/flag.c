/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:36:16 by martiper          #+#    #+#             */
/*   Updated: 2024/03/21 16:58:12 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "option/flag.h"
#include <stddef.h>
#include <stdlib.h>

void	cli_cleanup_flag(t_cli_flag *flag)
{
	free(flag->name);
	flag->option = NULL;
}
