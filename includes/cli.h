/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 11:37:34 by martiper          #+#    #+#             */
/*   Updated: 2024/03/25 23:28:27 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PUBLIC API

#ifndef CLI_H
# define CLI_H

#include "defs.h"

t_cli_handle	*cli_begin(const char* program_name, void* data);

#endif
