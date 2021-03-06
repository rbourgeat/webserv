/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:41:44 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/17 16:48:58 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Je refais tout le CGI au propre

#include "CGI.hpp"

CGI::CGI()
{
	_status = cgi_status::NON_INIT;
	_pipe = -1;
	_child_pid = -1;
	_child_return = 0;
}

CGI::~CGI()
{
	if (_pipe != -1)
		close(_pipe);
}

int CGI::get_pid() const
{ 
	return (_child_pid); 
}

int CGI::unset_pid()
{ 
	return _child_pid = -1; 
}

int CGI::get_fd() const
{ 
	return (_pipe); 
}

//  Alternative to poll checking
bool CGI::isPipeEmpty(int fd) const
{
	unsigned long bytesAvailable = 0;
	if (ioctl(fd, FIONREAD, &bytesAvailable) < 0)
	{
		bytesAvailable = 0;
	}
	return bytesAvailable == 0;
}

cgi_status::status CGI::status()
{
	if (_status == cgi_status::NON_INIT ||
			_status == cgi_status::DONE || IS_ERROR(_status))
	{
		return _status;
	}

	// CHECK TIMEOUT SYSTEM !!!

	int ret = waitpid(_child_pid, &_child_return, WNOHANG);
	// WNOHANG = go back if no thread
	if (ret == _child_pid) {
		if (BAD_EXIT(_child_return))
		{
			_status = cgi_status::CGI_ERROR;
		} else {
			_status = cgi_status::DONE;
		}
		unset_pid();
	} 
	else if (ret < 0) 
	{
		_status = cgi_status::SYSTEM_ERROR;
	} 
	else if (ret == 0 && _status != cgi_status::READABLE 
			&& isPipeEmpty(_pipe) == false ) 
	{
		_status = cgi_status::READABLE;
	}

	return (_status);
}

std::string CGI::execute(std::string PATH, HTTPRequest &request, PollFd &vPfd)
{
	_status = cgi_status::NON_INIT;
	//(void)vPfd;
	int fd1[2];
	int fd_in = 0;
	for (size_t j(0); j < request.body.size(); j++)
	{
		std::cout << request.body[j];
	}	

	if (request.rL.method == "POST")
	{
		for (int l(0); l < 2; l++)
		{
			std::cout << "l = " << l << std::endl;
			if (pipe(fd1) < 0)
			{
				_status = cgi_status::SYSTEM_ERROR;
				return ("Error");
			}

			size_t i = 0;
			char *cgi = strdup(PATH.c_str());
			if (cgi == NULL) //  + check if file is good
			{
				_status = cgi_status::SYSTEM_ERROR;
				free(cgi);
				return ("Error");
			}

			char *env[_variables.size() + 1];
			for (; i < _variables.size();)
			{
				env[i] = _variables[i];
				i++;
			}
			env[i] = NULL;

			char *args[] = {cgi, NULL};
			// Multi-threading....
			if ((_child_pid = fork()) < 0)
			{
				std::cerr << "System Error : fork()" << std::endl;
				_status = cgi_status::SYSTEM_ERROR;
				free(cgi);
				for (i = 0; i < _variables.size(); i++) {
					free(env[i]);
				}
				return ("Error");
			}
			if (_child_pid == 0)
			{
				for (size_t z(0); z < vPfd.getFdCount(); z++)
          			close(vPfd.getPfd()[z].fd);
				dup2(fd_in, 0);
				close(fd1[0]);
				if (dup2(fd1[1], 1) < 0)
				{
					free(cgi);
					close(fd1[1]);
					exit(-1);
				}
				close(fd1[1]);
				if (l == 0)
				{
					free(cgi);
					for (size_t j(0); j < request.body.size(); j++)
						std::cout << request.body[j];
					exit(0);
				}
				else
				{
					execve(args[0], args, env);
					free(cgi);
					exit(-1);
				}

			} else {
				wait(NULL);
				close(fd1[1]);
				dup2(fd1[0], 0);
				free(cgi);
				if (l == 0)
					fd_in = fd1[0];
				else
				{
					_status = cgi_status::WAITING;
					for (size_t k = 0; k < _variables.size(); k++)
					{
						free(env[k]);
					}
					_pipe = fd1[0];
					_buffSize = read(_pipe, _buffer, 10000);
					close(fd1[0]);
					close(fd_in);
					return (_buffer);
				}
			}
		}
	}
	else
	{
		if (pipe(fd1) < 0)
		{
			_status = cgi_status::SYSTEM_ERROR;
			return ("Error");
		}

		size_t i = 0;
		char *cgi = strdup(PATH.c_str());
		if (cgi == NULL) //  + check if file is good
		{
			_status = cgi_status::SYSTEM_ERROR;
			free(cgi);
			return ("Error");
		}

		char *env[_variables.size() + 1];
		for (; i < _variables.size();)
		{
			env[i] = _variables[i];
			i++;
		}
		env[i] = NULL;

		char *args[] = {cgi, NULL};
		// Multi-threading....
		if ((_child_pid = fork()) < 0)
		{
			std::cerr << "System Error : fork()" << std::endl;
			_status = cgi_status::SYSTEM_ERROR;
			free(cgi);
			for (i = 0; i < _variables.size(); i++) {
				free(env[i]);
			}
			return ("Error");
		}
		if (_child_pid == 0)
		{
			std::string exec_path = PATH;
			close(fd1[0]);
			if (dup2(fd1[1], 1) < 0)
			{
				close(fd1[1]);
				close(fd1[0]);
				free(cgi);
				exit(-1);
			}
			close(fd1[1]);
			execve(args[0], args, env);
			free(cgi);
			exit(-1);

		} else {
			wait(NULL);
			close(fd1[1]);
			dup2(fd1[0], 0);

			_status = cgi_status::WAITING;
			free(cgi);
			for (size_t k = 0; k < _variables.size(); k++)
			{
				free(env[k]);
			}
			_pipe = fd1[0];
			_buffSize = read(_pipe, _buffer, 10000);
			close(fd1[0]);
			return (_buffer);
		}
	}
	return ("");
}

std::string CGI::get_buffer_size(int count)
{
	std::ostringstream ss;
	std::string string;
	ss << (_buffSize - count);
	string = ss.str();
	return string;
}

int CGI::get_pipe() const
{ 
	return (_pipe); 
}

std::vector<char *> CGI::print_env()
{
	size_t i = 0;
	std::cout << "\n---------------------------" << std::endl;
	std::cout << "| CGI Environment Variables:" << std::endl;
	std::cout << "---------------------------" << std::endl;
	while (i < _variables.size())
	{
		std::cout << "| " << _variables[i] << std::endl;
		i++;
	}
	std::cout << "---------------------------" << std::endl;
	return (_variables);
}
