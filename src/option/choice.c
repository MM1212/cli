/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choice.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:36:16 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 21:18:41 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "option/choice.h"
#include <stddef.h>
#include <stdlib.h>

void	cli_cleanup_option_choice(t_cli_option_choice *choice)
{
	if (!choice)
		return ;
	free(choice->slug);
	ft_split_free(choice->aliases);
	free(choice);
}
