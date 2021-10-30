#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

#include "webserv.hpp";

struct	requestLine
{
				std::vector<char> element1;
				std::vector<char>	element2;
				std::vector<char>	element3;
};

class	HTTPMessage
{
				public:

				HTTPMessage()
				{
								isBody = false;
								isHeaderComplete = false;
								isFirstLineComplete = false;
								isMessageComplete = false;
				}
				~HTTPMessage(){;}

				void		insertHeaderLine()
				{
								if (isRequestLineComplete == false)
								{
										isRequestLineComplete = true;
								}
				}

				struct requestLine	rL;
				std::map<std::vector<char>, std::vector<char> > headerFields;
				std::vector<char>	body;
				std::vector<char>	tmp;
				bool							isBody;
				bool							isHeaderComplete;
				bool							isRequestLineComplete;
				bool							isMessageComplete;
};

#endif

