/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helloworld.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 12:58:22 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/22 16:51:54 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int main () {
   std::cout << "Content-type:text/html\r\n\r\n";
   std::cout << "<html>\n";
   std::cout << "<head>\n";
   std::cout << "<title>Hello World - First CGI Program</title>\n";
   std::cout << "</head>\n";
   std::cout << "<body>\n";
   std::cout << "<h2>Hello World! This is my first CGI program</h2>\n";
   std::cout << "</body>\n";
   std::cout << "</html>\n";
   
   return 0;
}