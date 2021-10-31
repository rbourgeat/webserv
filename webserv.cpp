/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 15:38:07 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/31 15:23:09 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPSocket.hpp"
#include "webserv.hpp"
#include "serverAndClient.hpp"
#include "config_parsing.cpp"
#include <unistd.h>

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

int		main(int argc, char const *argv[])
{
	const char *config_path; 
	if (argc != 2)
		config_path = "config/webserv.conf";
	else
		config_path = argv[1];
	/*Read config file*/
	std::vector<struct server> servers;
	std::vector<struct client> clients;
	readConfig(config_path, &servers);

	try
	{
		/*Create pollfd structure to monitor sockets with poll()*/
		PollFd vPfd;
		while(1)
		{
			poll(vPfd.getPfd().data(), vPfd.getFdCount(), 0); //Check all events on all fds at once with poll()
			if (vPfd.getFdCount() == 0)
			{
				for (size_t j(0); j < servers.size(); j++)
				{
					servers[j].sock = TCPSocket(servers[j].port);
					servers[j].sock.setToNonBlocking();
					servers[j].sock.socketBind();
					servers[j].sock.socketListen();
					vPfd.addFd(servers[j].sock.getSocketFd());
					std::cout << vPfd.getFdCount() << std::endl;
				}
			}
			for (size_t i(0); i < vPfd.getFdCount(); i++)
			{
				if (vPfd.getPfd()[i].revents & POLLIN) //if there are some data ready to be read
				{
					int k;
					if ((k = isServerFd(vPfd.getPfd()[i].fd, servers)) != -1)// == server.getSocketFd()) //if there is some data to read on server
					{
						std::cout << "+++++++ " << servers[k].name << " accepting new connection ++++++++\n\n";
						client c;
						c.socket = servers[k].sock.socketAccept();
						fcntl(c.socket, F_SETFL, O_NONBLOCK);
						c.servIndex = k;
						vPfd.addFd(c.socket);
						clients.push_back(c);
						std::cout << "+++New connection from fd " << c.socket << " accepted +++" << std::endl;
						std::cout << std::endl;
					}
					else //else, there is some data to read elsewhere (meaning on client side)
					{
						/*2 Receive request message (i.e., request for accessing a file OR executing a file (CGI program) to get the output) from client and parse it to analyze what is client expectation*/
						int k;
						k = findClient(vPfd.getPfd()[i].fd, clients);
						std::vector<unsigned char> request;
						std::cout << GRN << "+++ Request received from fd " << vPfd.getPfd()[i].fd << " +++\n";
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
							parseRequest(request, clients[k].request);
							for (size_t l(0); l < request.size(); l++)
								std::cout << GRN << request[l];
							std::cout << NC << std::endl;
							if (clients[k].request.isComplete == true)
							{
								std::vector<unsigned char> answer = parsing(clients[k].request, request, servers[clients[k].servIndex]);
								clients[k].answer = answer;
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
								std::cout << clients[k].answer.size() << std::endl;
								if (clients[k].answer.size() > 0)
								{
									servers[clients[k].servIndex].sock.socketSend(vPfd.getPfd()[i].fd, clients[k].answer);
									std::cout << MAG << "+++ Answer sent to fd " << vPfd.getPfd()[i].fd << " +++" << std::endl;
									for (size_t l(0); l < 75; l++)
										std::cout << MAG << clients[k].answer[l];
									clients[k].answer.clear();
									clients[k].request.clearAll();
									std::cout << NC << std::endl;
								}
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
		exit(EXIT_FAILURE);
	}
	return 0;
}
