#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include "webserv.hpp"

class TCPSocket
{
	public:
		TCPSocket(uint16_t port)
		{
			/*1. Creation of TCP socket with socket()*/
			_socketFd = socket(AF_INET, SOCK_STREAM, 0);
			if (_socketFd < 0)
				throw TCPSocketException("while creating");
            _address.sin_family = AF_INET;
            _address.sin_addr.s_addr = INADDR_ANY;
            _address.sin_port = htons(port);
            memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
		}
        ~TCPSocket() {;}

		void	socketBind()
		{
			/*2. Connect socket to a port with bind()*/
			if (bind(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
				throw TCPSocketException("while binding");
		}
		
		void	socketListen()
		{	
			/*3. Make server listen to incoming connections with listen()*/
			if (listen(_socketFd, 10) < 0)
				throw TCPSocketException("while listening");
		}

		void	socketConnect()
		{
			if (connect(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) <0)
				throw TCPSocketException("while connecting");
		}	

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
		int		_socketFd;
		struct	sockaddr_in _address;
		int		addrlen;

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
