/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIget.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 18:34:25 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/03 16:40:32 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.hpp"
# include "CGIparser.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    // You MUST print this out, otherwise the
    // server will not send the response:
    std::cout << "Content-type: text/plain\n"
              << std::endl;
    // For a CGI "GET," the server puts the data
    // in the environment variable QUERY_STRING:
    CGImap query(getenv("QUERY_STRING"));
    // Test: dump all names and values
    for (CGImap::iterator it = query.begin();
         it != query.end(); it++)
    {
        std::cout << (*it).first << " = "
                  << (*it).second << std::endl;
    }
    return (0);
}