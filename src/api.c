/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:10:33 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 15:25:03 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__handle_api.h"
#include "option/builder/__api.h"
#include "cli.h"
#include "option/utils.h"
#include <libft.h>
#include <stdio.h>
#include <assert.h>

#define this (g_cli_handle)

t_cli_handle *cli_begin(const char* program_name, void *data)
{
	ft_bzero(this, sizeof(t_cli_handle));
	this->program_name = program_name;
	this->free = cli_handle_cleanup;
	this->is_present = cli_handle_is_present;
	this->get_value = cli_handle_get_value;
	this->get_option = cli_handle_get_option;
	this->get_option_by_flag = cli_handle_get_option_by_flag;
	this->get_option_by_switch = cli_handle_get_option_by_switch;
	this->output_error = cli_handle_output_error;
	this->is_valid = cli_handle_is_valid;
	this->print = cli_handle_print;
	this->set_error = cli_handle_set_error;
	this->new_option = cli_handle_new_option;
	this->parse = cli_handle_parse;
	this->data = data;
	this->valid = true;
	return (this);
}

void cli_handle_print(void)
{
	ft_printf("CLI Handle:\n");
	ft_printf("\t- valid: %b\n", this->valid);
	if (this->error_code != 0)
		ft_printf("\t- error[%d]: %s\n", this->error_code, this->error_message);
	else
		ft_printf("\t- error: %s\n", "none");
	ft_printf("\t- options[%d]:\n", this->options_size);
	for (uint32_t i = 0; i < this->options_size; i++)
		cli_print_option(&this->options[i], true, 2);
	if (this->args)
	{
		size_t i;
		for (i = 0; this->args[i]; i++)
			;
		ft_printf("\t- arguments[%d]:\n", i);
		for (i = 0; this->args[i]; i++)
			ft_printf("\t\t- %s\n", this->args[i]);
	}
	else
		ft_printf("\t- arguments: %s\n", "none");
}

void cli_handle_set_error(t_cli_error_code code, ...)
{
	assert(code != 0 && "Error code must not be 0");
	this->error_code = code;
	if (this->error_message)
		free(this->error_message);
	char *main_error;
	switch (this->error_code)
	{
	case CLI_ERROR_BUILDER_INVALID_OPTION:
		main_error = CLI_ERROR_MSG_BUILDER_INVALID_OPTION;
		break;
	case CLI_ERROR_INVALID_OPTION:
		main_error = CLI_ERROR_MSG_INVALID_OPTION;
		break;
	case CLI_ERROR_UNRECOGNIZED_OPTION:
		main_error = CLI_ERROR_MSG_UNRECOGNIZED_OPTION;
		break;
	case CLI_ERROR_ARGUMENT_REQUIRED_FOR_OPTION:
		main_error = CLI_ERROR_MSG_ARGUMENT_REQUIRED_FOR_OPTION;
		break;
	case CLI_ERROR_INVALID_ARGUMENT:
		main_error = CLI_ERROR_MSG_INVALID_ARGUMENT;
		break;
	case CLI_ERROR_UNEXPECTED_ARGUMENT:
		main_error = CLI_ERROR_MSG_UNEXPECTED_ARGUMENT;
		break;
	case CLI_ERROR_AMBIGUOUS_OPTION:
		main_error = CLI_ERROR_MSG_AMBIGUOUS_OPTION;
		break;
	case CLI_ERROR_UNKNOWN:
	default:
		main_error = CLI_ERROR_MSG_UNKNOWN;
		break;
	}
	va_list args;
	char buffer[2048];

	va_start(args, code);
	ft_vsprintf(buffer, 2048, main_error, args);
	this->error_message = ft_strdup(buffer);
	va_end(args);
	this->valid = false;
}

t_cli_option *cli_handle_get_option(const char *name)
{
	for (uint32_t i = 0; i < this->options_size; i++)
	{
		if (ft_strcmp(this->options[i].slug, name) == 0)
			return (&this->options[i]);
	}
	return (NULL);
}

t_cli_option *cli_handle_get_option_by_flag(const char *flag)
{
	if (!this->options)
		return (NULL);
	for (uint32_t i = 0; i < this->options_size; i++)
	{
		for (uint32_t j = 0; j < this->options[i]._flags_size; j++)
		{
			if (this->options->flags & CLI_OPTION_FLAG_FUZZY)
			{
				if (ft_wildcard_match(this->options[i]._flags[j].name, flag))
					return (&this->options[i]);
			}
			else if (ft_strcmp(this->options[i]._flags[j].name, flag) == 0)
				return (&this->options[i]);
		}
	}
	return (NULL);
}

t_cli_option *cli_handle_get_option_by_switch(char letter)
{
	for (uint32_t i = 0; i < this->options_size; i++)
		for (uint32_t j = 0; j < this->options[i]._switches_size; j++)
		{
			if (this->options[i]._switches[j].letter == letter)
				return (&this->options[i]);
		}
	return (NULL);
}

bool cli_handle_is_present(const char *name)
{
	t_cli_option *option = cli_handle_get_option(name);
	if (!option)
		return (false);
	return (option->is_present);
}

char *cli_handle_get_value(const char *name)
{
	t_cli_option *option = cli_handle_get_option(name);
	if (option)
		return (option->value);
	return (NULL);
}

void cli_handle_cleanup(void)
{
	if (this->error_message)
		free(this->error_message);
	for (uint32_t i = 0; i < this->options_size; i++)
		cli_cleanup_option(&this->options[i]);
	free(this->options);
	free(this->options_runtime);
	ft_split_free(this->args);
	ft_bzero(this, sizeof(t_cli_handle));
}

bool cli_handle_is_valid(void)
{
	return (this->valid);
}

int cli_handle_output_error(void)
{
	if (this->error_code == 0)
		return (0);

	ft_fprintf(2, "[%d] %s: %s\n", this->error_code, this->program_name, this->error_message);
	free(this->error_message);
	this->error_message = NULL;
	return (this->error_code);
}

t_cli_option_builder *cli_handle_new_option(const char *slug, const char *desc, bool add_as_flag)
{
	t_cli_option_builder *builder = cli_opt_builder_init(this);
	builder->_option.slug = ft_strdup(slug);
	builder->set_description((char *)desc);
	builder->set_flags(CLI_OPTION_FLAG_NONE);
	if (add_as_flag)
		builder->add_flag((char *)slug);
	return (builder);
}
