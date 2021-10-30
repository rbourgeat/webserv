#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <vector>
#include <map>
#include <iostream>
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
			isHeaderComplete = false;
			isRequestLineComplete = false;
			isMessageComplete = false;
		}
		~HTTPRequest(){;}

		void		insertHeaderLine()
		{
			size_t i(0);
			size_t j(0);
			std::vector<unsigned char> temp1;
			std::vector<unsigned char> temp2;
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
				temp1 = std::vector<unsigned char>(tmp.begin() + (i - j), tmp.begin() + i);
				j = 0;
				if (i < tmp.size() && tmp[i] == ':')
					i++;
				if (tmp[i] == ' ')
					i++;
				while (i < tmp.size() && tmp[i] != ' ' && tmp[i] != '\r')
				{
					i++;
					j++;
				}
				temp2 = std::vector<unsigned char>(tmp.begin() + (i - j), tmp.begin() + i);
				headerFields.insert(std::pair<std::vector<unsigned char>, std::vector<unsigned char> >(temp1, temp2));
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
		struct requestLine								rL;
		std::map<std::vector<unsigned char>, std::vector<unsigned char> > headerFields;
		std::vector<unsigned char>						body;
		std::vector<unsigned char>						tmp;
		bool											isBody;
		bool											isHeaderComplete;
		bool											isRequestLineComplete;
		bool											isMessageComplete;
};

#endif

