#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include "webserv.hpp"

class TCPSocket
{
	public:
		TCPSocket(uint16_t port) : _port(port)
		{
			_socketFd = socket(AF_INET, SOCK_STREAM, 0); /*1*/
			if (_socketFd < 0)
				throw TCPSocketException("while creating");
            _address.sin_family = AF_INET;
            _address.sin_addr.s_addr = INADDR_ANY;
            _address.sin_port = htons(_port);
            memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
			//fcntl(_socketFd, F_SETFL, O_NONBLOCK); //server socket must be set to non-blocking
		}
        ~TCPSocket() {;}

		void	socketBind() /*2*/
		{
			if (bind(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
				throw TCPSocketException("while binding");
		}
		
		void	socketListen()
		{	
			if (listen(_socketFd, 10) < 0)
				throw TCPSocketException("while listening");
			std::cout << "Server listening on port: " << _port << std::endl;
		}

		void	socketConnect()
		{
			if (connect(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) <0)
				throw TCPSocketException("while connecting");
		}

		int		socketSend(int fd, void *message, unsigned short len)
		{
			int sent;
			unsigned short networkLen = htons(len);
			if (send(fd, reinterpret_cast<const char*>(& networkLen), sizeof(networkLen), 0) < 0)
				throw TCPSocketException("while sending");
			if ((sent = send(fd, reinterpret_cast<const char*>(message), len, 0)) < 0)
				throw TCPSocketException("while sending");
			return (sent);
		}

		int		socketRecv(int fd, std::vector<unsigned char>& buff)
		{
			int recved(0);
			int ret;
			unsigned short packetSize;
			
			if ((ret = recv(fd, reinterpret_cast<char*>(&packetSize), sizeof(packetSize), 0)) < 0 || ret != sizeof(unsigned short))
				throw TCPSocketException("while receiving");
			packetSize = ntohs(packetSize);
			buff.resize(packetSize);
			while (recved < packetSize)
			{
				if ((ret = recv(fd, reinterpret_cast<char*>(&buff[recved]), (packetSize - recved) * sizeof(unsigned char), 0)) < 0)
					throw TCPSocketException("while receiving");
				recved += ret;
			}
			return (recved);
		}

		/*GETTERS*/
		struct sockaddr_in const&	getAddress() const
		{
			return (_address);
		}
		int const& 					getSocketFd() const
		{
			return (_socketFd);
		}

	private:
		TCPSocket();
		int			_socketFd;
		struct		sockaddr_in _address;
		int			addrlen;
		uint16_t	_port;

	public:
		class TCPSocketException: public std::exception
	{
		public:
			TCPSocketException(std::string const& errorM) throw() : _errorM(errorM) {;}
			virtual ~TCPSocketException() throw(){;}

			virtual const char* what() const throw()
			{
				return (_errorM.c_str());
			}

		private:
			std::string	_errorM;
	};
};

/*1. Creation of a TCP socket with the following arguments:
 * - Socket family: AF_INET for a socket IPv4
 * - Socket type: SOCK_STREAM for TCP
 * - Socket protocal: associated with SOCK_STREAM, so set to 0.
 */

/*2. When a socket is created with socket(), it exists in a name space but has no address assigned to it. bind() assigns the address specified by the sockaddr_in structure to the socket. This results in assigning a transport address to the socket (using a port).*/

#endif
