#include "TCPSocket.hpp"
#include "webserv.hpp"

int main()
{
	try
	{
		TCPSocket client(PORT);
		client.socketConnect();
	}
    catch(TCPSocket::TCPSocketException const& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
	}
	
}
