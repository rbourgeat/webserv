/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getcookie.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgeat <rbourgeat@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 15:34:24 by rbourgeat         #+#    #+#             */
/*   Updated: 2021/12/03 15:42:51 by rbourgeat        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int main(void)
{
    std::cout << "Content-type:text/html\r\n\r\n";
    char *cookie = getenv("HTTP_COOKIE");
    printf("<p>Liste des Cookies: %s</p>", cookie);
    return (0);
}
