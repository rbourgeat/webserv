#ifndef SERVERANDCLIENT_HPP
# define SERVERANDCLIENT_HPP

#include "TCPSocket.hpp"
#include "HTTPRequest.hpp"

struct			errorPage
{
	int			num;
	std::string	path;
};

struct  					server
{
	std::string 			name;
	int         			port;
	TCPSocket				sock;
	std::vector<errorPage>	error;
	std::string				root;
	size_t					max_body_size;
};

struct							client
{
	int							socket;
	int							servIndex;
	HTTPRequest					request;
	std::vector<unsigned char>	answer;
	int							sentBytes;
	int							totalSentBytes;
	int							bytesToSend;
};

void  readConfig(char const *argv, std::vector<server> &server);

#endif
