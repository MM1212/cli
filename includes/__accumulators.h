/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __accumulators.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:10:44 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 11:08:55 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ACCUMULATORS_H
# define __ACCUMULATORS_H

# include "defs.h"

extern t_cli_option_builder	*g_cli_opt_builder;
extern t_cli_handle			*g_cli_handle;

// @deprecated
t_cli_option_builder	*cli_acc_get_opt_builder(void);
// @deprecated
t_cli_handle			*cli_acc_get_handle(void);

#endif
