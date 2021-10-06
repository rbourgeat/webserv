#ifndef POLLFD_HPP
# define POLLFD_HPP

#include "webserv.hpp"

class	PollFd
{
		public:
				PollFd(void) : _fdCount(0), _pfdSize(0) {;}
				PollFd(int pfdSize) : _fdCount(0), _pfdSize(pfdSize)
				{
						if (_pfdSize > 0)
							_pfd = _myAllocator.allocate(_pfdSize);
				}
				//PollFd(PollFd const& src);
				~PollFd(void)
				{
					_myAllocator.deallocate(_pfd, _pfdSize);
				}

				//PollFd &operator=(PollFd const& src);

				void	addFd(int fd)
				{
						int i(0);
						struct pollfd *temp;
						if (_pfdSize == 0)
						{
              _pfd = _myAllocator.allocate(_pfdSize + 1);
							_pfdSize++;
						}
						if (_pfdSize > 0 && _pfdSize < (_fdCount + 1))
						{
							temp = _myAllocator.allocate(_pfdSize + 1);
							while (i < _fdCount)
							{
									temp[i].fd = _pfd[i].fd;
									temp[i].events = _pfd[i].events;
									temp[i].revents = _pfd[i].revents;
									i++;
							}
							temp[i].fd = fd;
							temp[i].events = POLLIN | POLLOUT;
							_myAllocator.deallocate(_pfd, _pfdSize);
							_pfd = temp;
							_pfdSize++;
						}
						else
						{
							_pfd[_fdCount].fd = fd;
							_pfd[_fdCount].events = POLLIN | POLLOUT;
						}
						_fdCount++;
				}

				void		deleteFd(int i)
				{
						_pfd[i] = _pfd[_fdCount - 1]; //replace this one by the one at the end
						_fdCount--;
				}

				struct	pollfd *getPfd() const
				{
						return (_pfd);
				}

				int							getFdCount() const
				{
						return (_fdCount);
				}

		private:
				struct pollfd *_pfd;
				int						_fdCount;
				int						_pfdSize;
				std::allocator<struct pollfd> _myAllocator;
};

#endif

