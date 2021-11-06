#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include "webserv.hpp"
#include "PollFd.hpp"

class TCPSocket
{
	public:
		
		TCPSocket() {;}
		
		TCPSocket(uint16_t port) : _port(port)
	{
		_socketFd = socket(AF_INET, SOCK_STREAM, 0); /*1*/
		if (_socketFd < 0)
			throw TCPSocketException("while creating");
		_address.sin_family = AF_INET;
		_address.sin_addr.s_addr = INADDR_ANY;
		_address.sin_port = htons(_port);
		memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
	}
		~TCPSocket() {;}

		TCPSocket	&operator=(TCPSocket const& src)
		{
			_socketFd = src._socketFd;
    	_address = src._address;
    	addrlen = src.addrlen;
    	_port = src._port;
			return (*this);
		}

		void	setToNonBlocking()
		{
			fcntl(_socketFd, F_SETFL, O_NONBLOCK); //server socket must be set to non-blocking
		}

		void	socketBind() /*2*/
		{
			if (bind(_socketFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
				throw TCPSocketException("while binding");
		}

		void	socketListen()
		{	
			if (listen(_socketFd, 10) < 0)
				throw TCPSocketException("while listening");
			std::cout << "Server listening on port: " << _port << std::endl << std::endl;
		}

		int	socketAccept()
		{
			int addrlen = sizeof(_address);
			int new_socket;

			if ((new_socket = accept(_socketFd, (struct sockaddr *)&_address, (socklen_t *)&addrlen)) < 0)
				throw TCPSocketException("while accepting");
			return (new_socket);
		}

		size_t		socketSend(int fd, std::vector<unsigned char> answer)
		{
			int sentBytes(0);
			size_t totalSentBytes(0);
			size_t bytesLeft = answer.size();

			if ((sentBytes = send(fd, &answer[0], bytesLeft, 0)) < 0)
				throw TCPSocketException("while sending");
			else
			{
				bytesLeft-= sentBytes;
				totalSentBytes+= sentBytes;
				while (totalSentBytes < answer.size())
				{
					if ((sentBytes = send(fd, &answer[0 + totalSentBytes], bytesLeft, 0)) < 0)
						throw TCPSocketException("while sending");
					bytesLeft-= sentBytes;
					totalSentBytes+= sentBytes;
				}

			}
			//std::cout << "*********************totalSentBytes**************" << totalSentBytes << std::endl;
			return (totalSentBytes);
		}

		std::vector<unsigned char>	socketRecv(int i, PollFd &objectPoll)
		{
			std::vector<unsigned char> request(30000);
			std::vector<unsigned char> buffer(10000);
			int totalBytes(0);

			int nbytes = recv(objectPoll.getPfd()[i].fd, &buffer[0], sizeof(buffer), MSG_DONTWAIT);	
			if (nbytes <= 0)
			{
				if (nbytes == 0)
					std::cout << NC << "Socket " << objectPoll.getPfd()[i].fd << " hung up -------------------------------------------\n";
				else
					throw TCPSocketException("while receiving");
				close(objectPoll.getPfd()[i].fd);
				objectPoll.deleteFd(i);
			}
			else
			{
				int j(0);
				for (int k(0); k < nbytes; k++)
				{
					request[j] = buffer[k];
					j++;
				}
				totalBytes+= nbytes;
				while (nbytes > 0)
				{
					nbytes = recv(objectPoll.getPfd()[i].fd, &buffer[0], sizeof(buffer), MSG_DONTWAIT);
					for (int k(0); k < nbytes; k++)
					{
						request[j] = buffer[k];
						j++;
					}
					totalBytes+= nbytes;
				}
			}
			return (request);
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
		int getAddrlen()
		{
			return (addrlen);
		}

	private:
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
				perror("");
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
