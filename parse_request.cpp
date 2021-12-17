#include "HTTPRequest.hpp"
#include "serverAndClient.hpp"

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

void	parseRequest(std::vector<unsigned char> message, HTTPRequest &request, size_t nbytes, struct server s)
{
	size_t i(0);
	std::string temp;

	if (request.isHeaderComplete == false)
	{
		while (i < nbytes && request.isHeaderComplete == false)
		{
			if ((message[i] == '\r' && message[i + 1] == '\n') || message[i] == '\n')
			{
				if (message[i] == '\r')
					i+= 2;
				else
					i++;
				if (request.tmp.size() == 0 || (request.tmp.size() == 1 && message[0] == '\n')) //in case there is only a /n as last caracter
				{
					request.isHeaderComplete = true;
					//printRequestHeader(request);
					request.determineIfBody();
					if (request.isBody == false)
					{
						//std::cout << "No body\n";
						request.isComplete = true;
					}
				}
				else if (message[i] == '\r' && message[i + 1] == '\n')
				{
					i+= 2;
					request.insertHeaderLine(s.extensions);
					request.isHeaderComplete = true;
					//printRequestHeader(request);
					request.determineIfBody();
					if (request.isBody == false)
					{
						//std::cout << "No body\n";
						request.isComplete = true;
					}
				}
				else
					request.insertHeaderLine(s.extensions);
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
		if (s.max_body_size > 0)
		{
			if (request.bodySize > s.max_body_size)
				request.isComplete = true;
		}
		if (request.isComplete == false && request.isChunked == false)
		{
			while (i < nbytes && request.body.size() < request.bodySize)
			{
				request.body.push_back(message[i]);
				// std::cout << "message[i] = " << message[i] << std::endl;
				//if (i % 9 == 0)
				//	std::cout << request.body.size() << " / " << request.bodySize << std::endl;
				i++;
			}
			if (request.body.size() == request.bodySize)
			{
				request.isComplete = true;
				//printRequestBody(request);
			}
		}
		if (request.isComplete == false && request.isChunked == true)
		{
			while (i < nbytes)
			{
				if (request.isChunkSizeComplete == false)
				{
					while (i < nbytes && message[i] != '\r')
					{
						request.chunkSizeStr.push_back(message[i]);
						i++;
					}
					if (message[i] == '\r')
					{
						while (i < nbytes && (message[i] == '\r' || message[i] == '\n'))
							i++;
						request.chunkSize = std::stoi(request.chunkSizeStr.c_str(), 0, 16);
						request.chunkSizeStr.clear();
						request.isChunkSizeComplete = true;
					}
				}
				if (request.isChunkSizeComplete == true)
				{
					if (request.chunkSize == 0)
					{
						request.isComplete = true;
						//printRequestBody(request);
					}
					else
					{
						while (i < nbytes && request.chunkData.size() < request.chunkSize)
						{
							request.chunkData.push_back(message[i]);
							i++;
						}
						if (request.chunkData.size() == request.chunkSize)
						{
							for (size_t j(0); j < request.chunkData.size(); j++)
								request.body.push_back(request.chunkData[j]);
							request.chunkData.clear();
							request.isChunkSizeComplete = false;
						}
						while (i < nbytes && (message[i] == '\r' || message[i] == '\n'))
							i++;
					}
				}
			}
		}
	}
}
