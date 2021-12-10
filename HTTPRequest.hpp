/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 17:04:46 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/10 16:59:03 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <vector>
#include <map>
#include <iostream>
#define NC "\e[0m"
#define GRN "\e[0;32m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"

struct	requestLine
{
	std::string	method;
	std::string	requestTarget;
	std::string	httpVersion;
};

class	HTTPRequest
{
	public:

		HTTPRequest()
		{
			isBody = false;
			isChunked = false;
			isHeaderComplete = false;
			isRequestLineComplete = false;
			isCGI = false;
			isUpload = false;
			isComplete = false;
			chunkSize = -1;
			bodySize = 0;
			isChunkSizeComplete = false;
		}
		~HTTPRequest(){;}

		void		clearAll()
		{
			isBody = false;
			body.clear();
			isHeaderComplete = false;
			isRequestLineComplete = false;
			isCGI = false;
			isUpload = false;
			rL.method.clear();
			rL.requestTarget.clear();
			rL.httpVersion.clear();
			isComplete = false;
			headerFields.clear();
			chunkSize = -1;
		}

		std::string defineScriptName(std::string str)
		{
			std::string scriptName(rL.requestTarget.begin(), rL.requestTarget.end());
			if (str == "?")
				return (std::string(scriptName, 0, scriptName.find("?")));
			else if (str == "/")
				return (std::string(scriptName, 0, scriptName.find_last_of("/")));
			else
				return (scriptName);
		}
		
		/*std::string defineScriptName()
		{
			std::string scriptName(rL.requestTarget.begin(), rL.requestTarget.end());
			return (std::string(scriptName, 0, scriptName.find("?")));
		}*/

		std::string	defineQueryString()
		{
			std::string queryString(rL.requestTarget.begin(), rL.requestTarget.end());
			if (queryString.find("?") != std::string::npos)
				return (std::string(queryString, queryString.find("?") + 1));
			else
				return ("");
		}

		std::string	definePathInfo(std::string str)
		{
			(void)str;
			std::string PathInfo(rL.requestTarget.begin(), rL.requestTarget.end());
			/*if (PathInfo.find(str) != std::string::npos)
				return (std::string(PathInfo, PathInfo.find(str) + 5));
			else
				return ("");*/
			if (str.find("/") != std::string::npos)
			{
				PathInfo.erase(0, PathInfo.find_last_of("/"));
				return(PathInfo);
			}
			else
				return("");
		}

		void		determineIfBody()
		{
			if (headerFields.find("Content-Length") != headerFields.end())
			{
				isBody = true;
				bodySize = std::stoi(headerFields.find("Content-Length")->second.c_str());
			}
			if (headerFields.find("Transfer-Encoding") != headerFields.end())
			{
				isBody = true;
				isChunked = true;
			}
			determineIfUpload();
		}
		

		void		determineIfUpload()
		{
			if (rL.method == "POST")
				if (headerFields.find("Content-Type") != headerFields.end())
				{
					if (headerFields.find("Content-Type")->second.find("boundary") != std::string::npos)
					{
						isUpload = true;
						boundary = headerFields.find("Content-Type")->second;
						std::size_t pt = boundary.find("=");
						boundary.erase(boundary.begin(), boundary.begin() + pt + 1);
						//std::cout << ">> boundary = " << boundary << std::endl;
					}
					else
						isUpload = false;
				}
		}

		void		insertHeaderLine(std::vector<std::string> s)
		{
			size_t i(0);
			std::string temp1;
			std::string temp2;
			if (isRequestLineComplete == false)
			{
				insertHeaderRequestLine(s);
				isRequestLineComplete = true;
			}
			else
			{
				while (i < tmp.size() && tmp[i] != ':')
				{
					temp1.push_back(tmp[i]);
					i++;
				}
				if (i < tmp.size() && tmp[i] == ':')
					i++;
				if (i < tmp.size() && tmp[i] == ' ')
					i++;
				while (i < tmp.size())
				{
					temp2.push_back(tmp[i]);
					i++;
				}
				headerFields[temp1] = temp2;
			}
			tmp.clear();
		}

	private:
		void	insertHeaderRequestLine(std::vector<std::string> s)
		{
			size_t i(0);

			while (i < tmp.size() && tmp[i] != ' ')
			{
				if (tmp[i] >= 32 && tmp[i] <= 126)
					rL.method.push_back(tmp[i]);
				i++;
			}
			if (i < tmp.size() && tmp[i] == ' ')
				i++;
			while (i < tmp.size() && tmp[i] != ' ')
			{
				rL.requestTarget.push_back(tmp[i]);
				i++;
			}

			// Check if CGI extension is Good 
			for (size_t j = 0; j < s.size(); j++)
			{
				//std::cout << "==========> " << s[j] << std::endl;
				if (rL.requestTarget.find(s[j]) != std::string::npos)
					isCGI = true;
			}
			/*(void)s;
			if (rL.requestTarget.find(".cgi") != std::string::npos
				|| rL.requestTarget.find(".php") != std::string::npos
				|| rL.requestTarget.find(".py") != std::string::npos)
					isCGI = true;*/
			if (i < tmp.size() && tmp[i] == ' ')
				i++;
			while (i < tmp.size() && tmp[i] != '\n')
			{
				rL.httpVersion.push_back(tmp[i]);
				i++;
			}
		}

	public: 
		struct requestLine					rL;
		std::map<std::string, std::string>	headerFields;
		std::vector<unsigned char>			body;
		size_t								bodySize;
		std::vector<unsigned char>			tmp;
		size_t								chunkSize;
		std::string							chunkSizeStr;
		bool								isChunkSizeComplete;
		std::vector<unsigned char>			chunkData;
		bool								isBody;
		bool								isCGI;
		bool								isUpload;
		bool								isChunked;
		bool								isHeaderComplete;
		bool								isRequestLineComplete;
		bool								isComplete;
		std::string							boundary;

};

#endif

