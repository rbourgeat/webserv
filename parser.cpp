/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 16:30:12 by rbourgea          #+#    #+#             */
/*   Updated: 2021/09/23 18:44:03 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "classWebserv.hpp"

std::string getFileContent(const std::string& path)
{
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  return content;
}

unsigned long int countFileChar(std::string string)
{
    char c;
    unsigned long int cc = 0;

    std::ifstream FILE;
    FILE.open(string);
    if (!FILE.fail())
    {
        while (1)
        {
            FILE.get(c);
            if (FILE.eof()) break;
            cc++;
        }
        FILE.close();
    }
    else
    {
        std::cout << "Counter: Failed to open file: " << string << std::endl;
    }

    return cc;
};

bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

const char* parsing(char *buffer)
{
	int i = 0, j = 0;
	std::string location = "directory";
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
		s.erase(0, pos + delimiter.length());
		j++;
	}
	
	std::cout << "location: " << location << std::endl;

	if (!IsPathExist(location))
	{
		message = "HTTP/1.1 404 Not Found\n";
	}
	else
	{
		message += "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
		message += countFileChar(location);
		message += "\n\n" + getFileContent(location);
	}

	const char* c = message.c_str();
	return (c);
}

