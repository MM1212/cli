/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __handle_api.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:58:03 by martiper          #+#    #+#             */
/*   Updated: 2024/03/22 15:12:23 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HANDLE_API_H
# define __HANDLE_API_H

#include <defs.h>
#include "__accumulators.h"

void					cli_handle_print(void);
void					cli_handle_set_error(t_cli_error_code code, ...);
bool					cli_handle_parse(int argc, char **argv);
bool					cli_handle_is_present(const char *name);
char					*cli_handle_get_value(const char *name);
t_cli_option			*cli_handle_get_option(const char *name);
t_cli_option			*cli_handle_get_option_by_flag(const char *flag);
t_cli_option			*cli_handle_get_option_by_switch(char letter);
void					cli_handle_cleanup(void);
bool					cli_handle_is_valid(void);
int						cli_handle_output_error(void);
t_cli_option_builder	*cli_handle_new_option(const char *slug, const char *desc, bool add_as_flag);

#endif
