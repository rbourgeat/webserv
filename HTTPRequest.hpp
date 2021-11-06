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
			rL.method.clear();
			rL.requestTarget.clear();
			rL.httpVersion.clear();
			isComplete = false;
			headerFields.clear();
			chunkSize = -1;
		}

		void		determineIfBody()
		{
			std::map<std::string, std::string>::iterator it;
			it = headerFields.find("Content-Length");
			if (it != headerFields.end())
			{
				isBody = true;
				bodySize = std::stoi(it->second.c_str());
			}
			it = headerFields.find("Transfer-Encoding");
			if (it != headerFields.end())
			{
				isBody = true;
				isChunked = true;
			}
		}

		void		insertHeaderLine()
		{
			size_t i(0);
			size_t j(0);
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
					i++;
					j++;
				}
				temp1 = std::string(tmp.begin() + (i - j), tmp.begin() + i);
				j = 0;
				if (i < tmp.size() && tmp[i] == ':')
					i++;
				if (tmp[i] == ' ')
					i++;
				while (i < tmp.size() && tmp[i] != ' ')
				{
					i++;
					j++;
				}
				temp2 = std::string(tmp.begin() + (i - j), tmp.begin() + i);
				headerFields.insert(std::pair<std::string, std::string>(temp1, temp2));
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
		bool								isChunked;
		bool								isHeaderComplete;
		bool								isRequestLineComplete;
		bool								isComplete;
};

#endif

