/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 15:38:07 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/08 12:21:02 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPSocket.hpp"
#include <unistd.h>
int		main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	/*1. Make the HTTP web server program available from the internet and manage the flow of requests*/
	int new_socket; //long valread;
	//char buf[1000];
	//std::vector<unsigned char> request(30000);
	sethostname("hello", 6);

	try
	{
		/*Make server available from the internet*/
		TCPSocket server(PORT);
		server.setToNonBlocking();
		server.socketBind();
		server.socketListen();
		int server_fd = server.getSocketFd();
		//struct sockaddr_in address = server.getAddress();

		/*Create pollfd structure to store the sockets to monitor with poll()*/
		PollFd vPfd;
		vPfd.addFd(server_fd);

		while(1)
		{
			/*int nbEvents = */poll(vPfd.getPfd().data(), vPfd.getFdCount(), -1); //Check all events on all fds at once with poll()
			//std::cout << CYN << nbEvents << NC << std::endl << std::endl;
			//vPfd.getPfd()[0].revents = POLLIN;
			for (size_t i(0); i < vPfd.getFdCount(); i++)
			{
				if (vPfd.getPfd()[i].revents & POLLIN) //if there are some data ready to be read
				{
					if (vPfd.getPfd()[i].fd == server_fd) //if there is some data to read on server
					{
						std::cout << "+++++++ Accepting new connection ++++++++\n\n";
						new_socket = server.socketAccept();
						vPfd.addFd(new_socket);
						std::cout << "+++New connection from fd " << new_socket << " accepted +++" << std::endl;
						std::cout << std::endl;
					}
					else //else, there is some data to read elsewhere (meaning on client side)
					{
						std::cout << CYN << "revents " << vPfd.getPfd()[i].revents << NC << std::endl;
						/*2 Receive request message (i.e., request for accessing a file OR executing a file (CGI program) to get the output) from client and parse it to analyze what is client expectation*/
						std::vector<unsigned char> request(30000);
						std::cout << GRN << "+++ Request received from fd " << vPfd.getPfd()[i].fd << " +++\n";
						request = server.socketRecv(i, vPfd);
						for (size_t k(0); k < request.size(); k++)
							std::cout << GRN << request[k];
						std::cout << NC << std::endl;
						if (request[0] && vPfd.getPfd()[i].fd & POLLOUT) //if client is ready to receive answer
						{
							/*3. Send response message to client: either succeed in fulfilling request (i.e., provide access to the file OR returns file execution output), or return appropriate error status code*/
							std::vector<unsigned char> answer = parsing(request);
							/*for (size_t k(0); k < answer.size(); k++)
							  std::cout << request[k];*/
							if (request[0] != '\r')
								server.socketSend(vPfd.getPfd()[i].fd, answer);
							std::cout << MAG << "+++ Answer sent to fd " << vPfd.getPfd()[i].fd << " +++" << std::endl;
							for (size_t k(0); k < answer.size(); k++)
								std::cout << request[k];
							std::cout << std::endl;
							//std::cout << NC << "End of exchange with fd " << vPfd.getPfd()[i].fd <<" ----------------------------------------------" << std::endl << std::endl;
							//close(vPfd.getPfd()[i].fd);
							//vPfd.deleteFd(i);
						}
					}
				}
			}
		}
		//RAPH, YOUR CODE STARTS HERE :)
		/*printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		  {
		  perror("In accept");
		  exit(EXIT_FAILURE);
		  }
		// char buffer[30000] = {0};
		std::vector<unsigned char> buffer(30000);*/
		/*2. Receive request message (i.e., request for accessing a file OR executing a file (CGI program) to get the output) from client and parse it to analyze what is client expectation*/
		/*valread = read( new_socket , &buffer[0], 30000);
		// printf("[%s]\n",buffer );
		// const char* message = parsing(buffer);
		std::vector<unsigned char> message = parsing(buffer);*/
		/*3. Send response message to client: either succeed in fulfilling request (i.e., provide access to the file OR returns file execution output), or return appropriate error status code*/
		/*if (buffer[0] != '\r')
		  write(new_socket , &message[0] , message.size());
		  printf("\n-------------------------------------\n");
		  close(new_socket);*/
	}
	catch(TCPSocket::TCPSocketException const& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}
