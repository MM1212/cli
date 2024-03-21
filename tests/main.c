/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:45:23 by martiper          #+#    #+#             */
/*   Updated: 2024/03/21 17:28:08 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cli.h>

int main(int argc, char **av)
{
	(void)argc;
	(void)av;

	t_cli_handle *cli = cli_begin();
	cli->new_option("help", "Display an help message")
		->add_flag("help")
		->add_switch('h')
		->add_switch('t')
		->add_flag("h")
		->end();

	cli->print();
	if (!cli->is_valid())
		cli->output_error();
	cli->free();
}
