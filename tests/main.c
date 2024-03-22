/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:45:23 by martiper          #+#    #+#             */
/*   Updated: 2024/03/22 15:51:38 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cli.h>

int main(int argc, char **av)
{
	(void)argc;
	(void)av;

	t_cli_handle *cli = cli_begin();
	cli->new_option("help", "Display an help message", true)
		->add_switch('h')
		->set_flags(CLI_OPTION_FLAG_OPTIONAL)
		->set_type(CLI_OPTION_INPUT)
		->end();
	cli->new_option("color", "Enable color output", true)
		->set_default_value("always")
		->add_choice("always")
		->add_choice("auto")
		->add_choice("never")
		->end();

	if (!cli->parse(argc, av))
	{
		const int exit_status = cli->output_error();
		cli->free();
		return exit_status;
	}
	cli->print();
	cli->free();
}
