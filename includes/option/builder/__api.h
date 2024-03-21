/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __api.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:14:40 by martiper          #+#    #+#             */
/*   Updated: 2024/03/21 12:42:53 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __API_H
#define __API_H

#include "defs.h"

t_cli_option_builder	*cli_opt_builder_set_type(t_cli_option_type type);
t_cli_option_builder	*cli_opt_builder_set_flags(int flags);
t_cli_option_builder	*cli_opt_builder_add_flags(int flags);
t_cli_option_builder	*cli_opt_builder_remove_flags(int flags);
t_cli_option_builder	*cli_opt_builder_set_description(char *description);
t_cli_option_builder	*cli_opt_builder_set_default_value(char *default_value);
t_cli_option_builder	*cli_opt_builder_set_choices(char *choices);
t_cli_option_builder	*cli_opt_builder_add_switch(char letter);
t_cli_option_builder	*cli_opt_builder_add_flag(char *name);
t_cli_option_builder	*cli_opt_builder_init(void);
t_cli_option_builder	*cli_opt_builder_reset(void);
t_cli_option			*cli_opt_builder_end(void);

#endif
