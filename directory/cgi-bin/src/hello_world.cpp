/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hello_world.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 12:58:22 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/11 12:58:40 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int		main(int argc, char **argv, char **envp) 
{
	std::cout << "Content-type: text/txt\n\n";
	std::cout << "Hello World!" << std::endl;

	std::cout << "argc: " << argc << std::endl;

	std::cout << "-----===== argv =====-----" << std::endl;
	while (*argv) {
		std::cout << *argv << std::endl;
		argv++;
	}
	std::cout << "-----===== envp =====-----" << std::endl;
	while (*envp) {
		std::cout << *envp << std::endl;
		envp++;
	}

	return (0);
}