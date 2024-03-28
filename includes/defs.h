/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 11:39:03 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 15:14:04 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFS_H
#define DEFS_H

#include <stdbool.h>
#include <stdint.h>
#include <libft.h>

typedef struct s_cli_switch t_cli_switch;
typedef struct s_cli_flag t_cli_flag;
typedef struct s_cli_option t_cli_option;
typedef struct s_cli_option_choice t_cli_option_choice;
typedef struct s_cli_option_builder t_cli_option_builder;
typedef void (*t_cli_option_cb)(t_cli_option *option, void *data);
typedef struct s_cli_parser t_cli_parser;
typedef struct s_cli_settings t_cli_settings;
typedef struct s_cli_handle t_cli_handle;
typedef enum e_cli_option_type t_cli_option_type;
typedef enum e_cli_option_flag t_cli_option_flag;
typedef enum e_cli_error_code t_cli_error_code;

struct s_cli_switch
{
	char letter;
	t_cli_option *option;
};

struct s_cli_flag
{
	char *name;
	t_cli_option *option;
};

enum e_cli_option_type
{
	CLI_OPTION_INPUT, // takes an argument (any input)
	CLI_OPTION_SELECT // takes an argument (from a list of choices)
};

enum e_cli_option_flag
{
	CLI_OPTION_FLAG_NONE = 1,   // does not take any argument (if given, will add to program args)
	CLI_OPTION_FLAG_OPTIONAL = 2, // does not require an argument
	CLI_OPTION_FLAG_REQUIRED = 4, // must have an argument
	CLI_OPTION_FLAG_FUZZY = 8,   // will try to match with wildcards
	CLI_OPTION_FLAG_SIGNED = 16,    // argument must be a number
	CLI_OPTION_FLAG_UNSIGNED = 32, // argument must be an unsigned number
	CLI_OPTION_FLAG_FLOAT = 64, // argument must be a float
};

enum e_cli_error_code
{
	CLI_ERROR_NONE = 0,
	CLI_ERROR_BUILDER_INVALID_OPTION,
	CLI_ERROR_INVALID_OPTION,
	CLI_ERROR_UNRECOGNIZED_OPTION,
	CLI_ERROR_ARGUMENT_REQUIRED_FOR_OPTION,
	CLI_ERROR_INVALID_ARGUMENT,
	CLI_ERROR_UNEXPECTED_ARGUMENT,
	CLI_ERROR_AMBIGUOUS_OPTION,
	CLI_ERROR_MEMORY,
	CLI_ERROR_UNKNOWN
};

#define CLI_ERROR_MSG_BUILDER_INVALID_OPTION "invalid option while building -- ‘%s’"
#define CLI_ERROR_MSG_INVALID_OPTION "invalid option -- ‘%s’"
#define CLI_ERROR_MSG_UNRECOGNIZED_OPTION "unrecognized option -- ‘%s’"
#define CLI_ERROR_MSG_ARGUMENT_REQUIRED_FOR_OPTION "option ‘%s’ requires an argument"
#define CLI_ERROR_MSG_INVALID_ARGUMENT "invalid argument ‘%s’ for ‘%s’\nValid arguments are:\n%s"
#define CLI_ERROR_MSG_UNEXPECTED_ARGUMENT "invalid argument ‘%s’, expected ‘%s’"
#define CLI_ERROR_MSG_AMBIGUOUS_OPTION "option ‘--%s’ is ambiguous; possibilities: %s"
#define CLI_ERROR_MSG_MEMORY "memory error"
#define CLI_ERROR_MSG_UNKNOWN "unknown error"

struct s_cli_option_choice
{
	char *slug;
	char **aliases;
};

struct s_cli_option
{
	char *slug;				  // slug name
	t_cli_option_type type;	  // input or select
	int flags;				  // required, optional, hidden
	char *description;		  // description for help
	char *description_footer; // extra information added after all the options
	char *default_value;	  // default value if not provided (is_present(...) will always return true if default is set)
	char *variable_hint;	  // hint for flag when receives an argument
	t_list *choices;		  // list of choices for select type
	// runtime values
	bool is_present;	   // if the option was provided by user
	uint32_t presence_idx; // index of the option that was provided by user
	char *value;		   // value provided by user

	// internal values
	t_cli_option_cb cb; // callback function, if provided, will be called
	t_cli_switch *_switches;		  // list of switches for this option
	uint32_t _switches_size;		  // number of switches
	t_cli_flag *_flags;				  // list of flags for this option
	uint32_t _flags_size;			  // number of flags
	char *aliases;					  // list of aliases for this option (comma separated, eg, "-h,--help,--test")
	t_cli_handle *_handle;			  // handle that owns this option
};

struct s_cli_option_builder
{
	t_cli_option_builder *(*set_type)(t_cli_option_type type);
	t_cli_option_builder *(*set_flags)(int flags);
	t_cli_option_builder *(*add_flags)(int flags);
	t_cli_option_builder *(*remove_flags)(int flags);
	bool (*has_flags)(int flags);
	t_cli_option_builder *(*set_description)(char *description);
	t_cli_option_builder *(*set_description_footer)(char *description_footer);
	t_cli_option_builder *(*set_default_value)(char *default_value);   // will also call set_flags(CLI_OPTION_FLAG_OPTIONAL)
	t_cli_option_builder *(*set_variable_hint)(char *variable_hint);   // will also call set_flags(CLI_OPTION_FLAG_OPTIONAL) if has_flags(CLI_OPTION_FLAG_NONE)
	t_cli_option_builder *(*set_aliases)(char *aliases); // comma separated list of aliases, eg, "-h,--help,--test"
	t_cli_option_builder *(*add_choice)(char *choice, char **aliases); // will also call set_type(CLI_OPTION_SELECT)
	t_cli_option_builder *(*add_switch)(char letter);
	t_cli_option_builder *(*add_flag)(char *name);
	t_cli_option *(*end)(void);
	t_cli_option_builder *(*reset)(void);
	bool (*is_valid)(void);
	t_cli_option_builder *(*set_cb)(t_cli_option_cb cb);

	// internal values
	t_cli_option _option;
};

struct s_cli_parser
{
	t_list *args;
	char **argv;
	int argc;
	uint32_t option_counter;
};

struct s_cli_settings
{
	bool should_exit;
	bool run_cb_only_once;
};

struct s_cli_handle
{
	t_cli_option *options;
	t_cli_option **options_runtime; // a view that will be sorted by arg presence but doesnt own the options
	uint32_t options_size;

	// runtime values
	void *data;
	t_cli_settings settings;
	t_cli_parser parser;
	char **args;
	bool valid;
	t_cli_error_code error_code;
	const char* program_name; // not-heap allocated
	char *error_message; // must be heap allocated

	// internal api
	void (*print)(void);
	void (*set_error)(t_cli_error_code code, ...);

	// public api
	bool (*parse)(int argc, char **argv);
	bool (*is_present)(const char *name);
	char *(*get_value)(const char *name);
	t_cli_option *(*get_option)(const char *slug);
	t_cli_option *(*get_option_by_flag)(const char *flag);
	t_cli_option *(*get_option_by_switch)(char letter);
	void (*free)(void);
	bool (*is_valid)(void);
	int (*output_error)(void); // outputs error message to stderr and returns error code
	t_cli_option_builder *(*new_option)(const char *slug, const char *description, bool add_as_flag);
};

#endif
