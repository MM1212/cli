/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:45:23 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 15:14:06 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cli.h>

int main(int argc, char **av)
{
	(void)argc;
	(void)av;

	t_cli_handle *cli = cli_begin("test",NULL);
	cli->new_option("help", "Display an help message", true)
		->add_switch('h')
		->add_flags(CLI_OPTION_FLAG_OPTIONAL | CLI_OPTION_FLAG_FLOAT)
		->set_type(CLI_OPTION_INPUT)
		->end();
	cli->new_option("color", "Enable color output", true)
		->set_default_value("always")
		->add_flag("he")
		->add_choice("always", (char*[]){"always", "yes", "force", NULL})
		->add_choice("never", (char*[]){"auto", "no", "none", NULL})
		->add_choice("auto", (char*[]){"auto", "tty", "if-tty", NULL})
		->end();
	cli->print();
	if (!cli->parse(argc, av))
	{
		const int exit_status = cli->output_error();
		cli->free();
		return exit_status;
	}
	cli->print();
	cli->free();
}
