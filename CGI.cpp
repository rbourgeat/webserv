/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:41:44 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/15 18:10:41 by rbourgea         ###   ########.fr       */
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

int CGI::execute(std::string const &path)
{
	_status = cgi_status::NON_INIT;

	int output[2];
	int input[2];
	// Openning file descriptors
	if (pipe(output) < 0 || pipe(input) < 0)
	{
		_status = cgi_status::SYSTEM_ERROR;
		return (-1);
	}

	size_t i = 0;
	char *cgi = strdup(path.c_str());
	if (cgi == NULL) //  + check if file is good
	{
		_status = cgi_status::SYSTEM_ERROR;
		free(cgi);
		return (-1);
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
	_child_pid = fork();
	if (_child_pid < 0)
	{
		std::cerr << "System Error : fork()" << std::endl;
		_status = cgi_status::SYSTEM_ERROR;
		free(cgi);
		for (i = 0; i < _variables.size(); i++) {
		free(env[i]);
		}
		return (-1);
	}
	
	if (_child_pid == 0)
	{
		// Path of cgi file:
		std::string exec_path = "42";
		
		if (dup2(output[1], 1) < 0 || dup2(input[0], 0) < 0)
		{
			close(input[1]);
			close(input[0]);
			close(output[1]);
			close(output[0]);
			exit(-1);
		}
		if (chdir(exec_path.c_str()) < 0)
			exit(-1);
		close(input[1]);
		close(input[0]);
		close(output[1]);
		close(output[0]);
		execve(args[0], args, env);
		exit(-1);

	} else {
		// Starting timer for waiting server
		// _cgiTimer.start();
		_status = cgi_status::WAITING;
		free(cgi);
		for (i = 0; i < _variables.size(); i++)
		{
			free(env[i]);
		}
		close(input[0]);
		close(output[1]);
		_pipe = output[0];
		fcntl(_pipe, F_SETFL, O_NONBLOCK);
		// check if method == POST || body is empty
		if (1)
		{
			close(input[1]);
			return (-1);
		}
		else
		{
			fcntl(input[1], F_SETFL, O_NONBLOCK);
			return input[1];
		}
	}

	return (1);
}

int CGI::get_pipe() const
{ 
	return (_pipe); 
}