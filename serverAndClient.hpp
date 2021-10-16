#include "TCPSocket.hpp"

struct  server
{
        std::string name;
        int         port;
        TCPSocket		sock;
};

struct	client
{
				int					socket;
				int					servIndex;
				std::vector<unsigned char> answer;
};

void  readConfig(char const *argv, std::vector<server> &server);


