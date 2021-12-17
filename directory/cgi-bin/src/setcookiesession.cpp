/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setcookiesession.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:49:18 by rbourgeat         #+#    #+#             */
/*   Updated: 2021/12/17 15:37:18 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Source: https://developer.mozilla.org/fr/docs/Web/HTTP/Cookies

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(void)
{
     std::cout << "Content-Type: text/html\r\n";
     std::cout << "Set-Cookie: 42GameDev=BestClub; Expires=Wed, 29 Sept 2100 00:00:00 GMT;\r\n\r\n";
     std::cout << "Creation du Cookie: nom = 42GameDev, value = BestClub";
     std::cout << "<br><a href='getcookie.cgi'>Clique ici pour verifier</a>";
     return 0;
}
