/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:53:46 by martiper          #+#    #+#             */
/*   Updated: 2024/03/21 16:52:10 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include <defs.h>

void	cli_cleanup_option(t_cli_option *option);
void	cli_print_option(t_cli_option *option, bool header, int indent);

#endif
