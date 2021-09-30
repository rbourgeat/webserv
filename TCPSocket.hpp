#include "webserv.hpp"

class TCPSocket
{
	public:
		TCPSocket()
		{
			_socketFd = socket(AF_INET, SOCK_STREAM, 0); //create a TCP socket
			if (_socketFd < 0)
				throw TCPSocketException("Error during socket creation");
			_address.sin_family = AF_INET;
			_address.sin_addr.s_addr = INADDR_ANY;
			_address.sin_port = htons(PORT);
			memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
		}
		~TCPSocket() {;}

		struct sockaddr_in const&	getAddress() const
		{
			return (_address);
		}

		int const& 					getSocketFd() const
		{
			return (_socketFd);
		}

	private:
		int	_socketFd;
		struct sockaddr_in _address;

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
			std::string _errorM;
	};
};
