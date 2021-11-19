/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:42:20 by rbourgea          #+#    #+#             */
/*   Updated: 2021/11/19 16:54:57 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "webserv.hpp"
# include <stdlib.h>

# define IS_ERROR(x) \
    (x == cgi_status::CGI_ERROR || x == cgi_status::SYSTEM_ERROR \
  || x == cgi_status::UNSUPPORTED || x == cgi_status::TIMEOUT)

# define BAD_EXIT(x) \
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
		std::vector<char *>	print_env();
		std::string execute(std::string PATH, HTTPRequest &request);
		std::string			get_buffer_size(int count);
		void add_variable(std::string name, std::string value)
		{
			if (value.size() == 0)
				value = "";
			std::cout << "name=" << name << " / value=" << value << std::endl;
			std::ostringstream ss;
			ss << name << "=" << value;
			setenv(name.c_str(), value.c_str(), 1);
			_variables.push_back(strdup(ss.str().c_str()));
		}

	private:
		cgi_status::status	_status;
		int					_pipe;
		int					_child_pid;
		int					_child_return;
		std::vector<char *>	_variables;
		char				_buffer[10000];
		ssize_t				_buffSize;
		bool				isPipeEmpty(int fd) const;
		int get_pipe() const;
};

#endif