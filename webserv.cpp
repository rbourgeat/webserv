/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 15:38:07 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/06 19:35:48 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPSocket.hpp"

int		main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	/*1. Make the HTTP web server program available from the internet and manage the flow of requests*/
	int new_socket; //long valread;
	//char buf[1000];
  std::vector<unsigned char> request(30000);
	
	try
	{
		/*Make server available from the internet*/
		TCPSocket server(PORT);
		server.setToNonBlocking();
		server.socketBind();
		server.socketListen();
		int server_fd = server.getSocketFd();
		struct sockaddr_in address = server.getAddress();

		/*Create pollfd structure to store the sockets to monitor with poll()*/
		PollFd socketPoll(1);
		socketPoll.addFd(server_fd);

		while(1)
		{
			poll(socketPoll.getPfd(), socketPoll.getFdCount(), -1); //Check all events on all fds at once with poll()
			for (int i(0); i < socketPoll.getFdCount(); i++)
			{
				if (socketPoll.getPfd()[i].revents & POLLIN) //if there are some data ready to be read
				{
					if (socketPoll.getPfd()[i].fd == server_fd) //if there is some data to read on server
					{
						std::cout << GRN << "+++++++ Accepting new connection ++++++++\n\n";
						new_socket = server.socketAccept();
						//add_to_pfd(pfd, new_socket, &fd_count, &pfd_size); //Add the socket fd to the poll of fds to monitor
						socketPoll.addFd(new_socket);
						std::cout << GRN << "+++New connection from : " << address.sin_port << " +++" << std::endl;
						std::cout << std::endl;
					}
					else //else, there is some data to read elsewhere (meaning on client side)
					{
						/*2 Receive request message (i.e., request for accessing a file OR executing a file (CGI program) to get the output) from client and parse it to analyze what is client expectation*/
			      request = server.socketRecv(i, socketPoll);
						std::cout << GRN << "+++ Request received from fd : " << socketPoll.getPfd()[i].fd << "+++\n";
            for (size_t k(0); k < request.size(); k++)
            	std::cout << NC << request[k];
						if (request[0] && socketPoll.getPfd()[i].fd & POLLOUT) //if client is ready to receive answer
						{
				 			/*3. Send response message to client: either succeed in fulfilling request (i.e., provide access to the file OR returns file execution output), or return appropriate error status code*/
						  std::vector<unsigned char> answer = parsing(request);
							std::cout << MAG << answer[0] << std::endl;
							if (request[0] != '\r')
								server.socketSend(socketPoll.getPfd()[i].fd, answer);
        			std::cout << "+++ Answer sent +++" << std::endl;
							std::cout << GRN << "\n-------------------------------------\n";
      				close(socketPoll.getPfd()[i].fd);
			        socketPoll.deleteFd(i);
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
			close(new_socket);
		}*/
	}
	catch(TCPSocket::TCPSocketException const& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}
