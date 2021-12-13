#ifndef POLLFD_HPP
# define POLLFD_HPP

#include <poll.h>

class	PollFd
{
		public:
				PollFd(void) {}
				//PollFd(PollFd const& src);
				~PollFd(void) {}

				//PollFd &operator=(PollFd const& src);

				void	addFd(int fd)
				{
					struct pollfd pfd;
					pfd.fd = fd;
					pfd.events = POLLIN | POLLOUT;
					
					_pfd.push_back(pfd);
				}

				void		deleteFd(int i)
				{
					std::vector<struct pollfd>::iterator it(_pfd.begin());
					for (int j(0); j < i; j++)
						it++;
					_pfd.erase(it);
				}

				std::vector<struct pollfd>&	getPfd()
				{
						return (_pfd);
				}

				size_t							getFdCount() const
				{
						return (_pfd.size());
				}

		private:
				std::vector<struct pollfd> _pfd;
};

#endif
