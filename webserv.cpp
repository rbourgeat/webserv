/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 15:38:07 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/18 15:22:22 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPSocket.hpp"
#include "webserv.hpp"
#include "serverAndClient.hpp"
#include "config_parsing.cpp"
#include <unistd.h>
#include "HTTPResponse.hpp"

PollFd vPfd;
std::vector<struct server> servers;
std::vector<struct client> clients;

void	sighandler(int signum)
{
	(void)signum;
	if (signum == SIGINT)
	{
		for (size_t i(0); i < vPfd.getFdCount(); i++)
			close(vPfd.getPfd()[i].fd);
		exit(signum);
	}
}


int		findClient(int fd, std::vector<struct client> clients)
{
	for (size_t i(0); i < clients.size(); i++)
	{
		if (fd == clients[i].socket)
			return (i);
	}
	return (-1);
}

int		isServerFd(int fd, std::vector<struct server> servers)
{
	for (size_t i(0); i < servers.size(); i++)
	{
		if (fd == servers[i].sock.getSocketFd())
			return (i);
	}
	return (-1);
}

int		checkHost(HTTPRequest &request, int index, std::vector<struct server> servers)
{
	size_t j(0);

	if (request.headerFields.find("Host") == std::map<std::string, std::string>::iterator(request.headerFields.end()))
		return (index);
	while (j < servers.size())
	{
		if (servers[j].port == servers[index].port)
		{
            if (request.headerFields.find("Host")->second.find("localhost") != std::string::npos)
				return (j);
			 if (request.headerFields.find("Host")->second.find("127.0.0.1") != std::string::npos)
                return (j);
			if (request.headerFields.find("Host")->second.find(servers[j].name) != std::string::npos)
					return (j);
		}
		j++;
	}
	return (-1);
}

bool	isPortFree(std::vector<struct server> servers, int port, int i)
{
	int j(0);

	while (j < i)
	{
		if (servers[j].port == port)
			return (false);
		j++;
	}
	return (true);
}

int		main(int argc, char const *argv[])
{
	const char *config_path; 
	if (argc != 2)
		config_path = "config/webserv.conf";
	else
		config_path = argv[1];
	/*Read config file*/
	readConfig(config_path, &servers);

	/*Create pollfd structure to monitor sockets with poll()*/		
	while(1)
	{
		signal(SIGINT, &sighandler);
		try
		{
			poll(vPfd.getPfd().data(), vPfd.getFdCount(), 0); //Check all events on all fds at once with poll()
			if (vPfd.getFdCount() == 0)
			{
				for (size_t j(0); j < servers.size(); j++)
				{
					servers[j].sock = TCPSocket(servers[j].port);
					servers[j].sock.setToNonBlocking();
					if (isPortFree(servers, servers[j].port, j))
					{
						servers[j].sock.socketBind();
						servers[j].sock.socketListen();
					}
					vPfd.addFd(servers[j].sock.getSocketFd());
					//std::cout << vPfd.getFdCount() << std::endl;
				}
			}
			for (size_t i(0); i < vPfd.getFdCount(); i++)
			{
				if (vPfd.getPfd()[i].revents & POLLIN) //if there are some data ready to be read
				{
					int k;
					if ((k = isServerFd(vPfd.getPfd()[i].fd, servers)) != -1)// == server.getSocketFd()) //if there is some data to read on server
					{
						//std::cout << "+++++++ " << servers[k].name << " accepting new connection ++++++++\n\n";
						client c;
						c.socket = servers[k].sock.socketAccept();
						fcntl(c.socket, F_SETFL, O_NONBLOCK);
						c.servIndex = k;
						c.statusCode = "0";
						vPfd.addFd(c.socket);
						clients.push_back(c);
						//std::cout << "+++New connection from fd " << c.socket << " accepted +++" << std::endl;
						//std::cout << std::endl;
					}
					else //else, there is some data to read elsewhere (meaning on client side)
					{
						/*2 Receive request message (i.e., request for accessing a file OR executing a file (CGI program) to get the output) from client and parse it to analyze what is client expectation*/
						int k;
						k = findClient(vPfd.getPfd()[i].fd, clients);
						std::vector<unsigned char> request;
						//std::cout << GRN << "+++ Request received from fd " << vPfd.getPfd()[i].fd << " +++\n";
						request = servers[clients[k].servIndex].sock.socketRecv(i, vPfd);
						if (request.size() <= 0)
						{
							std::vector<struct client>::iterator it(clients.begin());
							for (int m(0); m < k; m++)
								it++;
							clients.erase(it);
						}
						else
						{
							parseRequest(request, clients[k].request, servers[clients[k].servIndex].sock.nbytes, servers[clients[k].servIndex]/*.extensions*/);
							//for (size_t l(0); l < request.size(); l++)
							//	std::cout << GRN << request[l];
							// std::cout << NC << std::endl;
							if (clients[k].request.isComplete == true)
							{
								std::vector<unsigned char> answer;
								if ((clients[k].servIndex = checkHost(clients[k].request, clients[k].servIndex, servers)) < 0)
								{
									HTTPResponse response(clients[k].request, vPfd);
									response.defineResponse();
									clients[k].statusCode = response.sL.statusCode;
									answer = std::vector<unsigned char>(response.resp.begin(), response.resp.end());
								}
								else
								{
									clients[k].request.determineIfUpload();
									HTTPResponse response(clients[k].request, servers[clients[k].servIndex], vPfd);
									response.defineResponse();
									clients[k].statusCode = response.sL.statusCode;
									answer = std::vector<unsigned char>(response.resp.begin(), response.resp.end());
								}
								clients[k].answer = answer;
								clients[k].bytesToSend = answer.size();
								clients[k].totalSentBytes = 0;
							}
						}
					}
				}
				/*3. Send response message to client: either succeed in fulfilling request (i.e., provide access to the file OR returns file execution output), or return appropriate error status code*/
				if (i < vPfd.getFdCount())
				{
					if (vPfd.getPfd()[i].revents & POLLOUT)
					{
						int k;
						if ((k = findClient(vPfd.getPfd()[i].fd, clients)) != -1)
						{
							//std::cout << clients[k].answer.size() << std::endl;
							if (clients[k].answer.size() > 0)
							{
								clients[k].sentBytes = servers[clients[k].servIndex].sock.socketSend(vPfd.getPfd()[i].fd, clients[k].answer);
								clients[k].totalSentBytes+= clients[k].sentBytes;
								/*for (size_t l(0); l < clients[k].answer.size(); l++)
									std::cout << MAG << clients[k].answer[l];*/
								clients[k].answer.erase(clients[k].answer.begin(), clients[k].answer.begin() + clients[k].sentBytes);
								if (clients[k].answer.size() == 0)
								{
									//std::cout << MAG << "+++ Answer sent to fd " << vPfd.getPfd()[i].fd << " +++" << std::endl;
									if (clients[k].statusCode == "413")
									{
										close(vPfd.getPfd()[i].fd);
										vPfd.deleteFd(i);
									}
									if (clients[k].statusCode == "301")
										sleep(10);
									clients[k].answer.clear();
									clients[k].request.clearAll();
								}
							}
						}
					}
				}
			}
		}
		catch(TCPSocket::TCPSocketException const& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl;
		}
	}
	return 0;
}
