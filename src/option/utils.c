/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:54:25 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 15:53:30 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "option/utils.h"
#include <option/flag.h>
#include <option/choice.h>
#include <libft.h>

void	cli_cleanup_option(t_cli_option *option)
{
	if (option->slug)
		free(option->slug);
	if (option->description)
		free(option->description);
	if (option->default_value)
		free(option->default_value);
	if (option->variable_hint)
		free(option->variable_hint);
	if (option->aliases)
		free(option->aliases);
	if (option->description_footer)
		free(option->description_footer);
	if (option->choices)
		ft_lstclear(&option->choices, (void (*)(void *))cli_cleanup_option_choice);
	if (option->value)
		free(option->value);
	for (uint32_t i = 0; i < option->_flags_size; i++)
		cli_cleanup_flag(&option->_flags[i]);
	free(option->_switches);
	if (option->_flags)
		free(option->_flags);
}

static void	print_option_choice(t_cli_option_choice* choice, char *tabs)
{
	ft_printf("%s\t\t- ", tabs, choice->slug);
	for (uint32_t i = 0; choice->aliases[i]; i++)
		ft_printf("%s%s", i == 0 ? "" : ", ", choice->aliases[i]);
	ft_printf("\n");
}

void	cli_print_option(t_cli_option *option, bool header, int indent)
{
	char tabs[indent + 1];
	tabs[indent] = '\0';
	ft_memset(tabs, '\t', indent);
	if (header)
		ft_printf("%s- %s:\n", tabs, option->slug);
	ft_printf("%s\tis_present: %b\n", tabs, option->is_present);
	ft_printf("%s\tslug: %s\n", tabs, option->slug);
	ft_printf("%s\ttype: %d\n", tabs, option->type);
	ft_printf("%s\tflags: %d\n", tabs, option->flags);
	ft_printf("%s\tdescription: %s\n", tabs, option->description);
	ft_printf("%s\tdefault_value: %s\n", tabs, option->default_value);
	ft_printf("%s\tvariable_hint: %s\n", tabs, option->variable_hint);
	if (option->type == CLI_OPTION_SELECT)
	{
		ft_printf("%s\tchoices:\n", tabs);
		ft_lstiter2(option->choices, (t_lst_iter2)print_option_choice, tabs);
	}
	ft_printf("%s\tswitches: '", tabs);
	for (uint32_t i = 0; i < option->_switches_size; i++)
	{
		if (i > 0)
			ft_printf(", ");
		ft_printf("%c", option->_switches[i]);
	}
	ft_printf("'\n%s\tflags: '", tabs);
	for (uint32_t i = 0; i < option->_flags_size; i++)
	{
		if (i > 0)
			ft_printf(", ");
		ft_printf("%s", option->_flags[i].name);
	}
	ft_printf("'\n%s\tvalue: %s\n", tabs, option->value);
}
