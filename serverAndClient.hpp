#include "TCPSocket.hpp"

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
	std::vector<unsigned char>	answer;
};

void  readConfig(char const *argv, std::vector<server> &server);
