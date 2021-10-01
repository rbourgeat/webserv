/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 15:38:07 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/01 17:23:07 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TCPSocket.hpp"

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	/*1. Make the HTTP web server program available from the internet and manage the flow of requests*/
	int addrlen;
	int new_socket; long valread;
	try
	{
		/*1.1. Create a socket object that is bound to a port and listens to incoming connections on that port.*/
		TCPSocket server(PORT);
		server.socketBind();
		server.socketListen();
		int server_fd = server.getSocketFd();
		struct sockaddr_in address = server.getAddress();
		addrlen = sizeof(address);
		//OLD VERSION FROM ME
		/*while(1)
		{
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("In accept");
				exit(EXIT_FAILURE);
			}
<<<<<<< HEAD
			char buff[INET6_ADDRSTRLEN] = { 0 };
			std::string clientAddress = inet_ntop(address.sin_family, (void*)&(address.sin_addr), buff, INET6_ADDRSTRLEN);
			std::cout << "Connexion de " << clientAddress << ":" << address.sin_port << std::endl;
			//char buffer[30000] = {0};
			std::vector<unsigned char> buffer;*/
			/*2. Receive request message (i.e., request for accessing a file OR executing a file (CGI program) to get the output) from client and parse it to analyze what is client expectation*/
			//valread = read( new_socket , buffer, 30000);
			/*server.socketRecv(new_socket, buffer);
			std::cout << buffer[0] << std::endl;
			//printf("[%s]\n", buffer);
			//const char* message = parsing(buffer);*/
			/*3. Send response message to client: either succeed in fulfilling request (i.e., provide access to the file OR returns file execution output), or return appropriate error status code*/
			/*if (buffer[0] != '\r')
				write(new_socket , message , strlen(message));
			printf("\n-------------------------------------\n");*/
			/*char buf[10] = "Received!";
			server.socketSend(new_socket, buf, sizeof(buffer));
			close(new_socket);
		}*/
		while(1)
		{
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("In accept");
                exit(EXIT_FAILURE);
            }
			// char buffer[30000] = {0};
			std::vector<unsigned char> buffer(30000);
			/*2. Receive request message (i.e., request for accessing a file OR executing a file (CGI program) to get the output) from client and parse it to analyze what is client expectation*/
			valread = read( new_socket , &buffer[0], 30000);
			// printf("[%s]\n",buffer );
			// const char* message = parsing(buffer);
			std::vector<unsigned char> message = parsing(buffer);
			/*3. Send response message to client: either succeed in fulfilling request (i.e., provide access to the file OR returns file execution output), or return appropriate error status code*/
			if (buffer[0] != '\r')
				write(new_socket , &message[0] , message.size());
			printf("\n-------------------------------------\n");
			close(new_socket);
		}

	}
	catch(TCPSocket::TCPSocketException const& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}
