#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

struct	statusLine
{
	std::string	httpVersion;
	std::string	statusCode;
	std::string	reasonPhrase;
};

class HTTPResponse
{
	public:
		HTTPResponse(HTTPRequest &request, struct server serv): r(request), s(serv)
	{
		sL.httpVersion = "HTTP/1.1";
	}
		~HTTPResponse(){;}

		void	createStatusLine()
		{
			if (s.root.size() > 0)
				fileLocation = s.root; //I think here this should be 1 instead of 0 --> to check
			else
				location = "directory";
			if (checkFile() == true)
			{
				location+= r.rL.requestTarget;
				if (location == (s.root + "/")) //REDIRECTION TO INDEX.HTML => TO DEFINE IN CONFIG FILE
				{
					fileLocation+=
				}
			}

		}

	private:

		std::string errorPageLocation(int code)
		{
			std::string location, codeToString;
			std::stringstream ss;
			ss << code;
			ss >> codeToString;
			size_t i(0);

			contentType = "text/html";
			while (i < s.error.size() && s.error[i].num != code)
				i++;
			if (i < s.error.size() && s.error[i].num == code)
				location = "directory/" + s.error[i].path;
			else
				location = "directory/" + codeToString + ".html";
			return (location);
		}

		bool isPathExist(const std::string &s)
		{
			struct stat buffer;
			return (stat (s.c_str(), &buffer) == 0);
		}

		bool checkFilePerm(std::string& path)
		{
			int errno;

			if (access (path.c_str(), F_OK) != 0)
			{
				if (errno == 2) //2 = ENOENT --> No such file or directory.
					return (1);
				else if (errno == 13) //13 = EACCES --> Permission denied.
					return (0);
			}
			return (1);
		}

		bool	checkFile()
		{
			if (!isPathExist(fileLocation))
			{                
				sL.statusCode = "404";
				sL.reasonPhrase = "Not found";
				fileLocation = errorPageLocation(404);
				return (true);
			}
			if (!checkFilePerm(fileLocation))
			{
				sL.statusCode = "403";
				sL.reasonPhrase = "Forbidden";
				fileLocation = errorPageLocation(403);
				return (true);
			}
			return (false);
		}

		bool	checkRequest()
		{
			if (r.rL.method.size() == 0 || r.rL.requestTarget.size() == 0 || r.rL.httpVersion.size() == 0)
			{
				sL.statusCode = "400";
				sL.reasonPhrase = "Bad request";
				fileLocation = errorPageLocation(400);
				return (true);
			}
			if (r.rL.method != "GET" && r.rL.method != "POST" && r.rL.method != "DELETE")
			{
				sL.statusCode = "405";
				sL.reasonPhrase = "Method not allowed";
				fileLocation = errorPageLocation(405);
				return (true);
			}
			if (r.rL.httpVersion.find("1.1") == std:string::npos)
			{
				sL.statusCode = "505";
				sL.reasonPhrase = "HTTP version not supported";
				fileLocation = errorPageLocation(505);
				return (true);
			}
			return (false);
		}

	public:
		HTTPRequest			r;
		struct server		s;
		struct statusLine   sL;
		std::string			fileLocation;
		std::string			contentType;
		std::string			contentLength;
};

#endif
