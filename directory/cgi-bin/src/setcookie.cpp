/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setcookie.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgeat <rbourgeat@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:49:18 by rbourgeat         #+#    #+#             */
/*   Updated: 2021/12/03 15:50:43 by rbourgeat        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Source: https://developer.mozilla.org/fr/docs/Web/HTTP/Cookies

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
     printf("Content-Type: text/html;\n");
     printf("Set-Cookie: %s=%s\r\n\r\n", "webserv", "42");
     printf("<br><a href='getcookie.cgi'>Clique ici pour verifier</a>");
     return 0;
}
