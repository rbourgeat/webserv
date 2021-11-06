#include "HTTPRequest.hpp"

void	printRequestHeader(HTTPRequest &request)
{
	std::map<std::string, std::string>::iterator it(request.headerFields.begin());
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
	while (it != request.headerFields.end())
	{
		std::cout << it->first << ": " << it->second << "\n";
		it++;
	}
	std::cout << std::endl;
}

void	printRequestBody(HTTPRequest &request)
{
	for (size_t i(0); i < request.body.size(); i++)
		std::cout << CYN << request.body[i];
	std::cout << std::endl;
}

void	parseRequest(std::vector<unsigned char> message, HTTPRequest &request)
{
	size_t i(0);
	size_t chunkSize(-1);
	std::string temp;

	if (request.isHeaderComplete == false)
	{
		while (i < message.size() && request.isHeaderComplete == false)
		{
			if (message[i] == '\r' && message[i + 1] == '\n')
			{
				i+= 2;
				if ((i - 2) == 0)
				{
					request.isHeaderComplete = true;
					printRequestHeader(request);
					request.determineIfBody();
					if (request.isBody == false)
					{
						std::cout << "No body\n";
						request.isComplete = true;
					}
				}
				else if (message[i] == '\r' && message[i + 1] == '\n')
				{
					i+= 2;
					request.insertHeaderLine();
					request.isHeaderComplete = true;
					printRequestHeader(request);
					request.determineIfBody();
					if (request.isBody == false)
					{
						std::cout << "No body\n";
						request.isComplete = true;
					}
				}
				else
					request.insertHeaderLine();
			}
			else
			{
				request.tmp.push_back(message[i]);
				i++;
			}
		}
	}
	if (request.isBody == true && request.isHeaderComplete == true && request.isComplete == false)
	{
		if (request.isChunked == false)
		{
			while (i < message.size() && request.body.size() < request.bodySize)
			{
				request.body.push_back(message[i]);
				i++;
			}
			if (request.body.size() == request.bodySize)
			{
				request.isComplete = true;
				printRequestBody(request);
			}
		}
		else
		{
			while (i < message.size() && request.isComplete == false)
			{
				while (i < message.size() && message[i] != '\r')
				{
					temp.push_back(message[i]);
					i++;
				}
				if (i < message.size() && message[i] == '\r' && message[i + 1] == '\n')
				{
					i+= 2;
					chunkSize = std::stoi(temp.c_str(), 0, 16);
					temp.clear();
				}
				if (chunkSize > 0)
				{
					size_t j(0);
					while (i < message.size() && j < chunkSize)
					{
						request.body.push_back(message[i]);
						i++;
						j++;
					}
				}
				else if (chunkSize == 0)
				{
					request.isComplete = true;
					printRequestBody(request);
				}
				while (i < message.size() && (message[i] == '\r' || message[i] == '\n'))
					i++;
			}
		}
	}
}
