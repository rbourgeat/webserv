/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cgi.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 13:51:49 by rbourgea          #+#    #+#             */
/*   Updated: 2021/10/01 18:14:25 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.hpp"

// /!\ NOTE /!\ : CGIpair is not protected (exemple: "%1H.")

// CGIpair will be used to represent a single name-value pair.
class CGIpair : public std::pair<std::string, std::string>
{
	public:
		CGIpair() { }
		CGIpair(std::string name, std::string value)
		{
			first = decodeURLString(name);
			second = decodeURLString(value);
		}
		// Auto conversion to bool
		operator bool() const
		{
			return (first.length() != 0);
		}

	private:
		static std::string decodeURLString(std::string URLstr)
		{
			const int len = URLstr.length();
			std::string result;
			for (int i = 0; i < len; i++)
			{
				if (URLstr[i] == '+')
					result += ' ';
				else if (URLstr[i] == '%')
				{
					result +=
						translateHex(URLstr[i + 1]) * 16 +
						translateHex(URLstr[i + 2]);
					i += 2;
				}
				else
					result += URLstr[i];
			}
			return result;
		}

		static char translateHex(char hex)
		{
			if (hex >= 'A')
				return (hex & 0xdf) - 'A' + 10;
			else
				return hex - '0';
		}
};

// CGImap will use CGIpair to parse each name-value pair that is submitted from the HTML 
// form into keys and values that it will hold in a map of strings.
class CGImap : public std::vector<CGIpair>
{
	public:
		CGImap(std::string query) : index(0), gq(query)
		{
			CGIpair p;
			while ((p = nextPair()) != 0)
				push_back(p);
		}
		// Look something up, as if it were a map:
		std::string operator[](const std::string &key)
		{
			iterator i = begin();
			while (i != end())
			{
				if ((*i).first == key)
					return (*i).second;
				i++;
			}
			return std::string(); // Empty string == not found
		}
		void dump(std::ostream &o, std::string nl = "<br>")
		{
			for (iterator i = begin(); i != end(); i++)
			{
				o << (*i).first << " = "
				<< (*i).second << nl;
			}
		}

	private:
		// Produces name-value pairs from the query
		// string. Returns an empty Pair when there's
		// no more query string left:
		CGIpair nextPair()
		{
			if (gq.length() == 0)
				return CGIpair(); // Error, return empty
			if (gq.find('=') == -1)
				return CGIpair(); // Error, return empty
			std::string name = gq.substr(0, gq.find('='));
			gq = gq.substr(gq.find('=') + 1);
			std::string value = gq.substr(0, gq.find('&'));
			gq = gq.substr(gq.find('&') + 1);
			return CGIpair(name, value);
		}

		std::string	gq;
		int			index;
		void operator=(CGImap &);
		CGImap(CGImap &);
	};

	// Helper class for getting POST data:
	class Post : public std::string
	{
	public:
		Post()
		{
			// For a CGI "POST," the server puts the
			// length of the content string in the
			// environment variable CONTENT_LENGTH:
			char *clen = getenv("CONTENT_LENGTH");
			if (clen == 0)
			{
				std::cout << "Zero CONTENT_LENGTH, Make sure "
							 "this is a POST and not a GET"
						  << std::endl;
				return;
			}
			int len = atoi(clen);
			char *s = new char[len];
			std::cin.read(s, len); // Get the data
			append(s, len);	  // Add it to this string
			delete[] s;
		}
};
