/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 15:38:07 by rbourgea          #+#    #+#             */
/*   Updated: 2021/09/26 14:42:57 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "classWebserv.hpp"

int main(int argc, char const *argv[])
{
	/*1. Make the HTTP web server program available from the internet and manage the flow of requests*/
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	
	// char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	memset(address.sin_zero, '\0', sizeof address.sin_zero);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		char buffer[30000] = {0};
		/*2. Receive request message (i.e., request for accessing a file OR executing a file (CGI program) to get the output) from client and parse it to analyze what is client expectation*/
		valread = read( new_socket , buffer, 30000);
		printf("[%s]\n",buffer );
		const char* message = parsing(buffer);
		/*3. Send response message to client: either succeed in fulfilling request (i.e., provide access to the file OR returns file execution output), or return appropriate error status code*/
		if (buffer[0] != '\r')
			write(new_socket , message , strlen(message));
		printf("\n-------------------------------------\n");
		close(new_socket);
	}
	return 0;
}
