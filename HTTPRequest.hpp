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
	std::vector<unsigned char>	method;
	std::vector<unsigned char>	requestTarget;
	std::vector<unsigned char>	httpVersion;
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
			isComplete = false;
			chunkSize = -1;
		}
		~HTTPRequest(){;}

		void		clearAll()
		{
			isBody = false;
			body.clear();
			isHeaderComplete = false;
			isRequestLineComplete = false;
			isCGI = false;
			rL.method.clear();
			rL.requestTarget.clear();
			rL.httpVersion.clear();
			isComplete = false;
			headerFields.clear();
			chunkSize = -1;
		}

		std::string defineScriptName()
		{
			std::string scriptName(rL.requestTarget.begin(), rL.requestTarget.end());
			return (std::string(scriptName, 0, scriptName.find("?")));
		}
		std::string	defineQueryString()
		{
			std::string queryString(rL.requestTarget.begin(), rL.requestTarget.end());
			return (std::string(queryString, queryString.find("?") + 1));
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
		}

		void		insertHeaderLine()
		{
			size_t i(0);
			std::string temp1;
			std::string temp2;
			if (isRequestLineComplete == false)
			{
				insertHeaderRequestLine();
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
				if (tmp[i] == ' ')
					i++;
				while (i < tmp.size() && tmp[i] != ' ')
				{
					temp2.push_back(tmp[i]);
					i++;
				}
				headerFields[temp1] = temp2;
			}
			tmp.clear();
		}

	private:
		void	insertHeaderRequestLine()
		{
			size_t i(0);
			size_t j(0);

			while (i < tmp.size() && tmp[i] != ' ')
			{
				i++;
				j++;
			}
			rL.method = std::vector<unsigned char>(tmp.begin() + (i - j), tmp.begin() + i);
			j = 0;
			if (i < tmp.size() && tmp[i] == ' ')
				i++;
			while (i < tmp.size() && tmp[i] != ' ')
			{
				i++;
				j++;
			}
			rL.requestTarget = std::vector<unsigned char>(tmp.begin() + (i - j), tmp.begin() + i);
			std::string rT(tmp.begin() + (i - j), tmp.begin() + i);
			if (rT.find(".cgi") != std::string::npos)
					isCGI = true;
			j = 0;
			if (i < tmp.size() && tmp[i] == ' ')
				i++;
			while (i < tmp.size() && tmp[i] != '\n')
			{
				i++;
				j++;
			}
			rL.httpVersion = std::vector<unsigned char>(tmp.begin() + (i - j), tmp.begin() + i);
		}

	public: 
		struct requestLine					rL;
		std::map<std::string, std::string>	headerFields;
		std::vector<unsigned char>			body;
		size_t								bodySize;
		std::vector<unsigned char>			tmp;
		int									chunkSize;
		std::vector<unsigned char>			chunkData;
		bool								isBody;
		bool								isCGI;
		bool								isChunked;
		bool								isHeaderComplete;
		bool								isRequestLineComplete;
		bool								isComplete;
};

#endif

