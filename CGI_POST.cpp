/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_POST.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 18:29:55 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/03 18:30:44 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.hpp"
# include "CGIparser.hpp"
# include <iostream>

int main()
{
    std::cout << "Content-type: text/plain\n"
         << std::endl;
    Post p; // Get the query string
    CGImap query(p);
    // Test: dump all names and values
    for (CGImap::iterator it = query.begin();
         it != query.end(); it++)
    {
        std::cout << (*it).first << " = "
             << (*it).second << std::endl;
    }
}