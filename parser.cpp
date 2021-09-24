/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 16:30:12 by rbourgea          #+#    #+#             */
/*   Updated: 2021/09/24 15:38:59 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "classWebserv.hpp"

std::string getFileContent(const std::string& path)
{
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  return content;
}

bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

std::string getContentType(std::string path)
{
	std::string extension = path.substr(path.find_last_of(".") + 1);
	
	if (extension == "html")
		extension = "text/html";
	else if (extension == "png")
		extension = "image/png";
	else
		extension = "text/plain";
	
	return (extension);
}

const char* parsing(char *buffer)
{
	int i = 0, j = 0;
	std::string location = "directory";
	std::string s(buffer);
	std::string delimiter = " ";
	std::string message;
	std::string error;

	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		if (j == 0 && (token != "GET" && token != "POST" && token != "DELETE"))
		{
			error = "405";
			break;
		}


		if (j == 1)
			location += token;
		s.erase(0, pos + delimiter.length());
		j++;
	}
	
	if (error == "405")
	{
		message += "HTTP/1.1 405 Method Not Allowed\nContent-Type: text/html";
		message += "\nContent-Length: ";
		location = "directory/405.html";
	}
	else if (!IsPathExist(location))
	{
		message += "HTTP/1.1 404 Not Found\nContent-Type: text/html";
		message += "\nContent-Length: ";
		location = "directory/404.html";
	}
	else if (location == "directory/")
	{
		message += "HTTP/1.1 200 OK\nContent-Type: text/html";
		message += "\nContent-Length: ";
		location = "directory/index.html";
	}
	else
	{
		message += "HTTP/1.1 200 OK\nContent-Type: ";
		message += getContentType(location);
		message += "\nContent-Length: ";
	}

	message += location.length();
	message += "\n\n" + getFileContent(location);

	const char* c = message.c_str();
	return (c);
}

