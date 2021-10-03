#include "TCPSocket.hpp"
#include "webserv.hpp"

int main()
{
	try
	{
		TCPSocket client(PORT);
		client.socketConnect();
		std::cout << "-----------Client connected----------\n";
		char str[12]("Hello there");
		std::vector<unsigned char> buff;
		send(client.getSocketFd(), &str, sizeof(&str), 0);
		/*client.socketSend(client.getSocketFd(), &str, sizeof(&str));
		client.socketRecv(client.getSocketFd(), buff);*/
		//std::cout << buff[0] << std::endl;
	}
    catch(TCPSocket::TCPSocketException const& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
	}
	
}
