/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   query_string.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 12:58:22 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/07 16:28:01 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

int main () {
   std::cout << "Content-type:text/html\r\n\r\n";
   std::cout << "<html>\n";
   std::cout << "<body>\n";
   char *data = getenv("QUERY_STRING");
   std::cout << "<h2>QUERY_STRING = " << data << "</h2>\n";
   std::cout << "</body>\n";
   std::cout << "</html>\n";
   
   return 0;
}