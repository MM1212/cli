/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 14:05:40 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 10:57:06 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__handle_api.h"
#include "option/builder/__api.h"
#include "cli.h"
#include "option/utils.h"
#include <libft.h>
#include <stdio.h>

#define this (g_cli_handle)

struct s_find_opt_choice
{
	char *arg;
	t_cli_option *option;
};

static bool find_option_choice(t_cli_option_choice *choice, struct s_find_opt_choice *data)
{
	for (uint32_t i = 0; choice->aliases[i]; i++)
	{
		if (data->option->flags & CLI_OPTION_FLAG_FUZZY)
		{
			if (ft_wildcard_match(choice->aliases[i], data->arg))
				return (true);
		}
		else if (ft_strcmp(choice->aliases[i], data->arg) == 0)
			return (true);
	}
	return (false);
}

static void cli_handle_invalid_arg_iter_choices(t_cli_option_choice *choice, char **valid_args)
{
	char *tmp = *valid_args;
	char aliases[1024];
	ft_bzero(aliases, sizeof(aliases));
	ft_sprintf(aliases, 1024, "  - ");
	for (uint32_t i = 0; choice->aliases[i]; i++)
	{
		if (i > 0)
			ft_strlcat(aliases, ", ", sizeof(aliases));
		ft_strlcat(aliases, "‘", sizeof(aliases));
		ft_strlcat(aliases, choice->aliases[i], sizeof(aliases));
		ft_strlcat(aliases, "’", sizeof(aliases));
	}
	if (*valid_args)
		*valid_args = ft_strjoin_sep("\n", *valid_args, aliases, NULL);
	else
		*valid_args = ft_strdup(aliases);
	free(tmp);
}

static bool parse_option(t_cli_option *option, char *arg, char *key)
{
	if (!option)
	{
		this->set_error(CLI_ERROR_UNRECOGNIZED_OPTION, key);
		return (false);
	}
	if (option->flags & CLI_OPTION_FLAG_REQUIRED && !arg)
	{
		this->set_error(CLI_ERROR_ARGUMENT_REQUIRED_FOR_OPTION, key);
		return (false);
	}
	if (option->flags & CLI_OPTION_FLAG_OPTIONAL && !arg)
		arg = option->default_value;
	if (option->type == CLI_OPTION_SELECT)
	{
		struct s_find_opt_choice data = {.arg = arg, .option = option};
		t_list *node = ft_lstfind(option->choices, (t_lst_find)find_option_choice, &data);
		if (!node)
		{
			char *valid_args = NULL;
			ft_lstiter2(option->choices, (t_lst_iter2)cli_handle_invalid_arg_iter_choices, &valid_args);
			this->set_error(CLI_ERROR_INVALID_ARGUMENT, arg ? arg : "", key, valid_args);
			free(valid_args);
			return (false);
		}
		t_cli_option_choice *choice = node->content;
		if (!(option->flags & CLI_OPTION_FLAG_FUZZY) && ft_strcmp(choice->slug, arg) != 0)
			arg = choice->slug;
	}
	if (
		arg &&
		((option->flags & CLI_OPTION_FLAG_SIGNED && !ft_isnbr(arg, true, false)) ||
		 (option->flags & CLI_OPTION_FLAG_UNSIGNED && !ft_isnbr(arg, false, false)) ||
		 (option->flags & CLI_OPTION_FLAG_FLOAT && !ft_isnbr(arg, true, true))))
	{
		char *expected = NULL;
		if (option->flags & CLI_OPTION_FLAG_SIGNED)
			expected = "number";
		else if (option->flags & CLI_OPTION_FLAG_UNSIGNED)
			expected = "unumber";
		else if (option->flags & CLI_OPTION_FLAG_FLOAT)
			expected = "float";
		this->set_error(CLI_ERROR_UNEXPECTED_ARGUMENT, arg ? arg : "", expected);
		return (false);
	}
	if (option->flags & CLI_OPTION_FLAG_MULTIPLE_VALUES)
	{
		if (!option->value)
			option->value = ft_strdup(arg);
		else
		{
			char *tmp = option->value;
			char sep[2] = {option->multiple_values_byte, '\0'};
			option->value = ft_strjoin_sep(sep, option->value, arg, NULL);
			free(tmp);
		}
	}
	else
	{
		if (option->value)
			free(option->value);
		option->value = ft_strdup(arg);
	}
	option->is_present = true;
	option->presence_idx = ++this->parser.option_counter;
	return (true);
}

static uint32_t check_if_flag_is_ambiguous(char *flag, char **options)
{
	uint32_t count = 0;
	uint32_t len = ft_strlen(flag);
	*options = NULL;
	bool exact_match = false;
	for (uint32_t i = 0; i < this->options_size; i++)
	{
		for (uint32_t j = 0; j < this->options[i]._flags_size; j++)
		{
			if (this->options[i].flags & CLI_OPTION_FLAG_FUZZY)
			{
				if (ft_wildcard_match(this->options[i]._flags[j].name, flag))
				{
					if (count == 0)
						*options = ft_strdup(this->options[i]._flags[j].name);
					else
					{
						char *tmp = *options;
						*options = ft_strjoin_sep(",", *options, this->options[i]._flags[j].name, NULL);
						free(tmp);
					}
					count++;
				}
			}
			else if (ft_strncmp(flag, this->options[i]._flags[j].name, len) == 0)
			{
				if (ft_strlen(this->options[i]._flags[j].name) == len)
				{
					count = 1;
					if (*options)
						free(*options);
					*options = ft_strdup(this->options[i]._flags[j].name);
					exact_match = true;
					break;
				}
				if (count == 0)
					*options = ft_strdup(this->options[i]._flags[j].name);
				else
				{
					char *tmp = *options;
					*options = ft_strjoin_sep(",", *options, this->options[i]._flags[j].name, NULL);
					free(tmp);
				}
				count++;
			}
		}
		if (exact_match)
			break;
	}
	return (count);
}

static bool output_ambiguous_options(char *flag, char *ambiguous_options)
{
	char **options = ft_split(ambiguous_options, ",");
	free(ambiguous_options);
	if (!options)
	{
		free(flag);
		this->set_error(CLI_ERROR_MEMORY);
		return (false);
	}
	for (uint32_t i = 0; options[i]; i++)
	{
		char *tmp = ft_strjoin(3, "'--", options[i], "'");
		if (!tmp)
		{
			ft_split_free(options);
			free(flag);
			this->set_error(CLI_ERROR_MEMORY);
			return (false);
		}
		free(options[i]);
		options[i] = tmp;
	}
	ambiguous_options = ft_strjoin_sep2(" ", options);
	ft_split_free(options);
	if (!ambiguous_options)
	{
		free(flag);
		this->set_error(CLI_ERROR_MEMORY);
		return (false);
	}
	this->set_error(CLI_ERROR_AMBIGUOUS_OPTION, flag, ambiguous_options);
	free(flag);
	free(ambiguous_options);
	return (false);
}

static bool parse_flag(char *str)
{
	char *original = str;
	str += 2; // skip --
	char *equal_sign = ft_strchr(str, '=');
	char *flag;
	if (!equal_sign)
		flag = ft_strdup(str);
	else
		flag = ft_substr(str, 0, equal_sign - str);
	uint32_t ambiguous_count = 0;
	char *ambiguous_options = NULL;
	if ((ambiguous_count = check_if_flag_is_ambiguous(flag, &ambiguous_options)) > 1)
		return output_ambiguous_options(flag, ambiguous_options);
	else
		free(ambiguous_options);
	t_cli_option *option = cli_handle_get_option_by_flag(flag);
	free(flag);
	if (!equal_sign)
		return parse_option(option, NULL, original);
	char *value = ft_strdup(equal_sign + 1);
	*equal_sign = '\0';
	bool valid = parse_option(option, value, original);
	free(value);
	return (valid);
}

static bool parse_switch(char *switch_seq, int *i)
{
	switch_seq++; // skip -
	while (*switch_seq)
	{
		t_cli_option *option = cli_handle_get_option_by_switch(*switch_seq);
		bool is_valid = false;
		char *switch_str = ft_substr(switch_seq, 0, 1);
		if (!option)
		{
			this->set_error(CLI_ERROR_UNRECOGNIZED_OPTION, switch_str);
			is_valid = false;
		}
		else if (option->flags & CLI_OPTION_FLAG_NONE)
		{
			is_valid = parse_option(option, NULL, switch_str);
			switch_seq++;
		}
		else
		{
			char *arg = switch_seq + 1;
			if (!*arg)
			{
				*i += 1;
				arg = this->parser.argv[(*i)];
				switch_seq++;
			}
			else
				switch_seq += ft_strlen(arg) + 1;
			is_valid = parse_option(option, arg, switch_str);
		}
		free(switch_str);
		if (!is_valid)
			return (false);
	}
	return (true);
}

static void cli_handle_callbacks(void)
{
	for (uint32_t i = 0; i < this->options_size; i++)
	{
		if (this->options_runtime[i]->cb && this->options_runtime[i]->is_present)
		{
			this->options_runtime[i]->cb(this->options_runtime[i], this->data);
			if (this->settings.run_cb_only_once)
				break;
		}
	}
}

static void cli_sort_options_by_presence(void)
{
	this->options_runtime = ft_calloc(this->options_size, sizeof(t_cli_option *));
	if (!this->options_runtime)
	{
		this->set_error(CLI_ERROR_MEMORY);
		return;
	}
	for (uint32_t i = 0; i < this->options_size; i++)
		this->options_runtime[i] = &this->options[i];
	for (uint32_t i = 0; i < this->options_size; i++)
	{
		for (uint32_t j = i + 1; j < this->options_size; j++)
		{
			if (this->options_runtime[i]->presence_idx > this->options_runtime[j]->presence_idx)
			{
				t_cli_option *tmp = this->options_runtime[i];
				this->options_runtime[i] = this->options_runtime[j];
				this->options_runtime[j] = tmp;
			}
		}
	}
}

bool cli_handle_parse(int argc, char **argv)
{
	argc--;
	argv++;
	this->parser.argc = argc;
	this->parser.argv = argv;
	bool valid = true;
	bool add_all_as_args = false;
	for (int i = 0; i < argc; i++)
	{
		bool is_flag = ft_strncmp(argv[i], "--", 2) == 0;
		bool is_only_flag = false;
		if (add_all_as_args || argv[i][0] != '-' || argv[i][1] == '\0' || (is_only_flag = ft_strcmp(argv[i], "--") == 0))
		{
			if (is_only_flag)
				add_all_as_args = true;
			ft_lstadd_back(&this->parser.args, ft_lstnew(argv[i]));
			continue;
		}
		valid = is_flag ? parse_flag(argv[i]) : parse_switch(argv[i], &i);
		if (!valid)
			break;
	}
	this->valid = valid;
	if (!valid)
	{
		ft_lstclear(&this->parser.args, NULL);
		return (false);
	}
	uint32_t count = ft_lstsize(this->parser.args);
	this->args = ft_calloc(count + 1, sizeof(char *));
	t_list *head = this->parser.args;
	for (uint32_t i = 0; i < count; i++)
	{
		this->args[i] = ft_strdup(head->content);
		head = head->next;
	}
	ft_lstclear(&this->parser.args, NULL);
	cli_sort_options_by_presence();
	cli_handle_callbacks();
	return (true);
}
