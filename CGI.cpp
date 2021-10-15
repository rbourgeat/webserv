/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:41:44 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/11 13:58:55 by rbourgea         ###   ########.fr       */
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