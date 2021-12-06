/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setcookie.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:49:18 by rbourgeat         #+#    #+#             */
/*   Updated: 2021/12/06 15:03:25 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Source: https://developer.mozilla.org/fr/docs/Web/HTTP/Cookies

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(void)
{
     std::cout << "Content-Type: text/html\r\n";
     std::cout << "Set-Cookie: webserv=42\r\n\r\n";
     std::cout << "Creation du Cookie: nom = webserv, value = 42";
     std::cout << "<br><a href='getcookie.cgi'>Clique ici pour verifier</a>";
     return 0;
}
