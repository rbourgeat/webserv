/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:42:20 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/11 13:58:00 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"

#define IS_ERROR(x) \
    (x == cgi_status::CGI_ERROR || x == cgi_status::SYSTEM_ERROR \
  || x == cgi_status::UNSUPPORTED || x == cgi_status::TIMEOUT)

#define BAD_EXIT(x) \
    (WIFEXITED(x) &&  WEXITSTATUS(x) != 0) || WIFSIGNALED(x)

namespace cgi_status
{
	enum status
	{
		NON_INIT,
		WAITING,
		DONE,
		CGI_ERROR,
		SYSTEM_ERROR,
		READABLE,
		UNSUPPORTED,
		TIMEOUT
	};
}

class CGI
{
	public:
		CGI();
		~CGI();
		int					get_pid() const;
		int					unset_pid();
		int					get_fd() const;
		cgi_status::status	status();

	private:
		cgi_status::status	_status;
		int					_pipe;
		int					_child_pid;
		int					_child_return;
		bool				isPipeEmpty(int fd) const;
};

#endif