#ifndef SERVERANDCLIENT_HPP
# define SERVERANDCLIENT_HPP

#include "TCPSocket.hpp"
#include "HTTPRequest.hpp"

struct			errorPage
{
	int			num;
	std::string	path;
};

struct			redirection
{
	int			num;
	std::string	path;
};

struct			location
{
	std::string					path;
	std::vector<std::string>	method;
	std::string					root;
	struct redirection			redi;
	std::string					defaultFile;
	std::string					uploadDir;
	std::string					dirList;
};

struct  					server
{
	std::string 			name;
	int         			port;
	TCPSocket				sock;
	std::vector<errorPage>	error;
	std::vector<location>	loc;
    std::vector<std::string>    extensions;
	size_t					max_body_size;
};

struct							client
{
	int							socket;
	int							servIndex;
	HTTPRequest					request;
	std::vector<unsigned char>	answer;
	std::string					statusCode;
	int							sentBytes;
	int							totalSentBytes;
	int							bytesToSend;
};

void  readConfig(char const *argv, std::vector<server> &server);

#endif
