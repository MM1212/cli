/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:17:05 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 23:11:49 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__accumulators.h"
#include "option/builder/__api.h"
#include <option/flag.h>
#include <option/choice.h>
#include <option/utils.h>
#include <libft.h>

#define this (g_cli_opt_builder)

t_cli_option_builder	*cli_opt_builder_init(t_cli_handle *handle)
{
	this->set_type = cli_opt_builder_set_type;
	this->set_flags = cli_opt_builder_set_flags;
	this->add_flags = cli_opt_builder_add_flags;
	this->remove_flags = cli_opt_builder_remove_flags;
	this->has_flags = cli_opt_builder_has_flags;
	this->set_description = cli_opt_builder_set_description;
	this->set_description_footer = cli_opt_builder_set_description_footer;
	this->set_default_value = cli_opt_builder_set_default_value;
	this->set_variable_hint = cli_opt_builder_set_variable_hint;
	this->add_choice = cli_opt_builder_add_choice;
	this->add_switch = cli_opt_builder_add_switch;
	this->add_flag = cli_opt_builder_add_flag;
	this->is_valid = cli_opt_builder_is_valid;
	this->reset = cli_opt_builder_reset;
	this->end = cli_opt_builder_end;
	this->set_cb = cli_opt_builder_set_cb;
	this->reset();
	this->_option._handle = handle;

	this->add_flags(CLI_OPTION_FLAG_NONE);
	return (this);
}

t_cli_option			*cli_opt_builder_end(void)
{
	if (!this->is_valid())
	{
		this->reset();
		return (NULL);
	}
	if (this->_option._handle->get_option(this->_option.slug))
	{
		t_cli_option *opt = this->_option._handle->get_option(this->_option.slug);
		cli_cleanup_option(opt);
		*opt = this->_option;
		this->reset();
		return (opt);
	}
	uint32_t		new_len = this->_option._handle->options_size + 1;
	t_cli_option	*new_options = ft_calloc(new_len, sizeof(t_cli_option));
	ft_memmove(
		new_options,
		this->_option._handle->options,
		this->_option._handle->options_size * sizeof(t_cli_option)
	);
	free(this->_option._handle->options);
	this->_option._handle->options = new_options;
	this->_option._handle->options_size = new_len;
	this->_option._handle->options[new_len - 1] = this->_option;
	ft_bzero(&this->_option, sizeof(t_cli_option));
	return (&new_options[new_len - 1]);
}

t_cli_option_builder	*cli_opt_builder_reset(void)
{
	t_cli_handle *handle = this->_option._handle;
	cli_cleanup_option(&this->_option);
	ft_bzero(&this->_option, sizeof(t_cli_option));
	this->_option._handle = handle;
	return (this);
}

t_cli_option_builder	*cli_opt_builder_set_type(t_cli_option_type type)
{
	this->_option.type = type;
	if (type == CLI_OPTION_INPUT) {
		ft_lstclear(&this->_option.choices, (void(*)(void*))cli_cleanup_option_choice);
		this->_option.choices = NULL;
	}
	return (this);
}

t_cli_option_builder	*cli_opt_builder_set_flags(int flags)
{
	this->_option.flags = flags;
	return (this);
}

t_cli_option_builder	*cli_opt_builder_add_flags(int flags)
{
	this->_option.flags |= flags;
	return (this);
}

t_cli_option_builder	*cli_opt_builder_remove_flags(int flags)
{
	this->_option.flags &= ~flags;
	return (this);
}

bool	cli_opt_builder_has_flags(int flags)
{
	return ((this->_option.flags & flags) == flags);
}

t_cli_option_builder	*cli_opt_builder_set_description(char *description)
{
	if (this->_option.description)
		free(this->_option.description);
	this->_option.description = ft_strdup(description);
	return (this);
}

t_cli_option_builder	*cli_opt_builder_set_description_footer(char *description_footer)
{
	if (this->_option.description_footer)
		free(this->_option.description_footer);
	this->_option.description_footer = ft_strdup(description_footer);
	return (this);
}

t_cli_option_builder	*cli_opt_builder_set_default_value(char *default_value)
{
	this->_option.default_value = ft_strdup(default_value);
	this->remove_flags(CLI_OPTION_FLAG_REQUIRED)
		->add_flags(CLI_OPTION_FLAG_OPTIONAL);
	return (this);
}

t_cli_option_builder	*cli_opt_builder_set_variable_hint(char *variable_hint)
{
	this->_option.variable_hint = ft_strdup(variable_hint);
	if (this->has_flags(CLI_OPTION_FLAG_NONE))
		this->set_flags(CLI_OPTION_FLAG_OPTIONAL);
	return (this);
}

t_cli_option_builder	*cli_opt_builder_add_choice(char *choice, char **aliases)
{
	if (this->_option.type != CLI_OPTION_SELECT)
		this->set_type(CLI_OPTION_SELECT);
	t_cli_option_choice *new = ft_calloc(1, sizeof(t_cli_option_choice));
	if (!new)
	{
		this->_option._handle->set_error(CLI_ERROR_MEMORY);
		return (this);
	}
	new->slug = ft_strdup(choice);
	if (!new->slug)
	{
		free(new);
		this->_option._handle->set_error(CLI_ERROR_MEMORY);
		return (this);
	}
	uint32_t count;
	for (count = 0; aliases[count]; count++);
	new->aliases = ft_calloc(count + 1, sizeof(char *));
	if (!new->aliases)
	{
		free(new->slug);
		free(new);
		this->_option._handle->set_error(CLI_ERROR_MEMORY);
		return (this);
	}
	for (uint32_t i = 0; i < count; i++)
	{
		new->aliases[i] = ft_strdup(aliases[i]);
		if (!new->aliases[i])
		{
			cli_cleanup_option_choice(new);
			this->_option._handle->set_error(CLI_ERROR_MEMORY);
			return (this);
		}
	}
	t_list* node = ft_lstnew(new);
	if (!node)
	{
		cli_cleanup_option_choice(new);
		this->_option._handle->set_error(CLI_ERROR_MEMORY);
		return (this);
	}
	ft_lstadd_back(&this->_option.choices, node);
	return (this);
}

t_cli_option_builder	*cli_opt_builder_add_switch(char letter)
{
	t_cli_switch	*new_switches = ft_calloc(this->_option._switches_size + 1, sizeof(t_cli_switch));
	ft_memmove(
		new_switches,
		this->_option._switches,
		this->_option._switches_size * sizeof(t_cli_switch)
	);
	free(this->_option._switches);
	this->_option._switches = new_switches;
	this->_option._switches[this->_option._switches_size++].letter = letter;
	return (this);
}

t_cli_option_builder	*cli_opt_builder_add_flag(char *name)
{
	t_cli_flag	*new_flags = ft_calloc(this->_option._flags_size + 1, sizeof(t_cli_flag));
	ft_memmove(
		new_flags,
		this->_option._flags,
		this->_option._flags_size * sizeof(t_cli_flag)
	);
	free(this->_option._flags);
	this->_option._flags = new_flags;
	this->_option._flags[this->_option._flags_size++].name = ft_strdup(name);
	return (this);
}

bool	cli_opt_builder_is_valid(void)
{
	if (!this->_option.slug)
		this->_option._handle->set_error(CLI_ERROR_BUILDER_INVALID_OPTION, "Missing slug");
	else if (this->_option.type == CLI_OPTION_SELECT && !this->_option.choices)
		this->_option._handle->set_error(CLI_ERROR_BUILDER_INVALID_OPTION, "Missing choices");
	else if (!this->_option._switches && !this->_option._flags)
		this->_option._handle->set_error(CLI_ERROR_BUILDER_INVALID_OPTION, "Missing switches or flags");
	else if (!this->_option.description)
		this->_option._handle->set_error(CLI_ERROR_BUILDER_INVALID_OPTION, "Missing description");
	return (this->_option._handle->valid);
}

t_cli_option_builder*	cli_opt_builder_set_cb(t_cli_option_cb cb)
{
	this->_option.cb = cb;
	return (this);
}
