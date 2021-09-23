/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 16:30:12 by rbourgea          #+#    #+#             */
/*   Updated: 2021/09/23 17:24:21 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "classWebserv.hpp"

char* get_file()
{

}

bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

const char* parsing(char *buffer)
{
	int i = 0, j = 0;
	std::string location = "/directory";
	std::string s(buffer);
	std::string delimiter = " ";
	std::string message;

	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		
		if (j == 1)
			location += token;
		
		// std::cout << std::endl << token << std::endl << std::endl;
		
		s.erase(0, pos + delimiter.length());
		j++;
	}

	std::cout << std::endl << location << std::endl << std::endl;
	
	if (!IsPathExist(location))
		message = "HTTP/1.1 404 OK\nContent-Type: text/plain\nContent-Length: 100\n\nError 404: File Not Found !";
		
	
	const char* c = message.c_str();
	return (c);
}

