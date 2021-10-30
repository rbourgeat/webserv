#include "HTTPRequest.hpp"

void	parseRequest(std::vector<unsigned char> message, HTTPRequest &request)
{
	size_t i(0);

	if (request.isHeaderComplete == false)
	{
		while (i < message.size() && request.isHeaderComplete == false)
		{
			if (message[i] == '\r')
			{
				i++;
				if (message[i] == '\n')
				{			
					i++;
					if (message.size())
						request.isHeaderComplete = true;
					else
					{
						request.insertHeaderLine();
						//request.tmp.push_back(message[i]);
					}
				}
			}
			else
			{
				request.tmp.push_back(message[i]);
				i++;
			}
		}
	}
	std::cout << CYN << "PARSE REQUEST" << std::endl;
	for (size_t i(0); i < request.rL.method.size(); i++)
		std::cout << request.rL.method[i];
	std::cout << " ";
	for (size_t i(0); i < request.rL.requestTarget.size(); i++)
        std::cout << request.rL.requestTarget[i];
	std::cout << " ";
	for (size_t i(0); i < request.rL.httpVersion.size(); i++)
        std::cout << request.rL.httpVersion[i];
	std::cout << std::endl;
}
