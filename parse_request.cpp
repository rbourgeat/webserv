#include "HTTPMessage.hpp"

void	parse_request(std::vector<char> message, class HTTPMessage request)
{
				if (request.isHeaderComplete == false)
				{
				for (int i(0); i < message.size(); i++)
				{
						if (message[i] == '\r')
						{
							i++;
								if (message[i] == '\n')
								{			
									i++;
									if (i == message.size())
											isHeaderComplete = true;
									else
									{
											request.insertHeaderLine();
											request.tmp.push_back(message[i]);
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
}
