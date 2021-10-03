/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 15:38:07 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/03 15:46:22 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPSocket.hpp"

void	add_to_pfd(struct pollfd pfd[5], int new_socket, int *fd_count, int *pfd_size)
{
	//will have to manage the size of my structure as necessary; basic for the moment, I have 5 places
	//(*pfd_size) = ...
	(void)pfd_size;
	pfd[*fd_count].fd = new_socket;
	pfd[*fd_count].events = POLLIN | POLLOUT;
	(*fd_count)++;
}

void	del_from_pfd(struct pollfd pfd[], int i, int *fd_count)
{
	pfd[i] = pfd[*fd_count - 1];
	(*fd_count)--;
}

int		main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	/*1. Make the HTTP web server program available from the internet and manage the flow of requests*/
	int addrlen;
	int new_socket; //long valread;
	int fd_count(1);
	//char buf[1000];
	int pfd_size(5); //arbitrary; will have to manage it more appropriately;
	std::vector<unsigned char> request(30000);
	int totalBytes(0);
	struct pollfd pfd[pfd_size]; //this structure array stores the socket fds to monitor
	try
	{
		/*1.1. Create a socket object that is bound to a port and listens to incoming connections on that port.*/
		TCPSocket server(PORT);
		server.setToNonBlocking();
		server.socketBind();
		server.socketListen();
		int server_fd = server.getSocketFd();
		struct sockaddr_in address = server.getAddress();
		addrlen = sizeof(address);

		pfd[0].fd = server_fd; //socket fd to monitor
		pfd[0].events = POLLIN | POLLOUT; //events expected
		while(1)
		{
			//THIS PART IS UNDER CONTRUCTION, SO IT'S COMMENTED
			/*int num_events = */poll(pfd, fd_count, -1); //Check all events on all fds at the same time thanksto poll()
			for (int i(0); i < fd_count; i++)
			{
				if (pfd[i].revents & POLLIN) //if there are some data ready to be read
				{
					if (pfd[i].fd == server_fd) //if there is some data to read on server
					{
						std::cout << "+++++++ Accepting new connection ++++++++\n\n";
						if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
						{
							perror("In accept");
							exit(EXIT_FAILURE);
						}
						add_to_pfd(pfd, new_socket, &fd_count, &pfd_size); //Add the socket fd to the poll of fds to monitor
						std::cout << "+++New connection from : " << address.sin_port << " +++" << std::endl;
						std::cout << std::endl;
					}
					else //if there is some data to read elsewhere (meaning on client side)
					{
						//std::cout << "HEEEEEEEY\n";
			      std::vector<unsigned char> buffer(30000);
						int nbytes = recv(pfd[i].fd, &buffer[0], sizeof(buffer), 0); //recv/ read data
						//std::cout << "nbytes = " << nbytes << std::endl;
						if (nbytes <= 0)
						{
							if (nbytes == 0)
								std::cout << "Socket " << pfd[i].fd << " hung up.\n";
							else
								perror("recv");
							close(pfd[i].fd);
							del_from_pfd(pfd, i, &fd_count);
						}
						else
						{
								int j(0);
								for (int k(0); k < nbytes; k++)
								{
												request[j] = buffer[k];
												j++;
								}
								totalBytes+= nbytes;
								while (nbytes > 0)
								{
									nbytes = recv(pfd[i].fd, &buffer[0], sizeof(buffer), MSG_DONTWAIT);
									for (int k(0); k < nbytes; k++)
									{
													request[j] = buffer[k];
													j++;
									}
									totalBytes+= nbytes;
								}
								std::cout << "+++ Request received +++\n";
            		for (int k(0); k < totalBytes; k++)
               		std::cout << request[k];
								if (pfd[i].fd & POLLOUT)
								{
										std::cout << "+++ Answer sent +++\n"; 
							      std::vector<unsigned char> answer = parsing(request);
										if (request[0] != '\r')
        								send(pfd[i].fd , &answer[0] , answer.size(), 0);
        						printf("\n-------------------------------------\n");
      							close(pfd[i].fd);
			              del_from_pfd(pfd, i, &fd_count);
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
	}
	catch(TCPSocket::TCPSocketException const& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}
