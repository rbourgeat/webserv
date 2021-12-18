/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 17:04:43 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/18 16:52:10 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "webserv.hpp"
# include "CGI.hpp"
# include "serverAndClient.hpp"
# include <sys/time.h>
# include "PollFd.hpp"


struct	statusLine
{
	std::string	httpVersion;
	std::string	statusCode;
	std::string	reasonPhrase;
};

class HTTPResponse
{
	public:

		HTTPResponse(HTTPRequest &request, PollFd &vPfd): r(request), _vPfd(vPfd)
	{
		sL.httpVersion = "HTTP/1.1";
		invalidHost = true;
	}
		HTTPResponse(HTTPRequest &request, struct server serv, PollFd &vPfd): r(request), s(serv), _vPfd(vPfd)
	{
		sL.httpVersion = "HTTP/1.1";
		invalidHost = false;
	}
		~HTTPResponse(){;}

		void	defineResponse()
		{
			if (!checkHost())
			{
				getLoc();
				if (loc.redi.num > -1)
					redirectClient();
				else if (!checkRequest() && !checkClientBodySize() && !checkMethod())
				{
					fileLocation = loc.root;
					fileLocation+= r.rL.requestTarget;
					if (!r.isCGI)
					{
						if (r.isUpload)
						{
							foreachUpload();
						}
						else
						{
							struct stat s;
							stat(fileLocation.c_str(), &s);
							if (s.st_mode & S_IFDIR)
							{
								if (loc.dirList == "ON")
									defineResponseForCGI();
								else
								{
									if (isPathExist(fileLocation))
									{
										fileLocation = loc.root + "/" + loc.defaultFile;
										defineResponseHeaderForNonCGI();
										if (contentLength.size() > 0)
											body = getFileContent(fileLocation);
									}
									else
									{
										sL.statusCode = "404";
										sL.reasonPhrase = "Not found";
										errorPageLocation(404);
									}
								}
							}
							else
							{
								defineResponseHeaderForNonCGI();
								if (contentLength.size() > 0)
									body = getFileContent(fileLocation);
							}
						}
					}
					else
						defineResponseForCGI();
				}
			}
			aggregateResponse();
		}

	private:

		void	getLoc()
		{
			int	comp(0);

			if (r.rL.requestTarget.size() > 0)
			{
				for (size_t i(0); i < s.loc.size(); i++)
				{
					int n(0);
					size_t j(0);
					while (j < s.loc[i].path.size() && j < r.rL.requestTarget.size() &&
							s.loc[i].path[j] == r.rL.requestTarget[j])
					{
						j++;
						n++;
					}
					if (n > comp && j == s.loc[i].path.size())
					{
						loc = s.loc[i];
						comp = n;
					}
				}
			}
		}

		void	redirectClient()
		{
			std::stringstream ss;
			ss << loc.redi.num;
			ss >> sL.statusCode;
			if (loc.redi.num == 301)
				sL.reasonPhrase = "Moved permanently";
			redirectionLocation = loc.redi.path;
		}
		void	aggregateResponse()
		{
			resp+= sL.httpVersion + " " + sL.statusCode + " " + sL.reasonPhrase + "\r\n";
			if (!isPathExist(fileLocation) && !r.isCGI && loc.redi.num == -1)
				resp+="Content-Length: 0\r\n\r\n";
			else
			{
				if (r.rL.method != "DELETE" && loc.redi.num == -1)
				{
					if (r.isUpload && sL.statusCode != "413")
						resp+= "Content-Length: 13 \r\n\r\nUpload Done !";
					else
						resp+= "Content-Length: " + contentLength + "\r\n";
					if (contentType.size() > 0)
					{
						resp+= "Content-Type: " + contentType + "\r\n";
						resp+= "\r\n";
					}
					resp+= body;
				}
				else
				{
					if (redirectionLocation.size() > 0)
						resp+= "Location: " + redirectionLocation + "\r\n";
					resp+= "\r\n";
				}
			}
		}

		bool		checkHost()
		{
			if (invalidHost == true)
			{
				sL.statusCode = "400";
				sL.reasonPhrase = "Bad request";
				errorPageLocation(400);
				return (true);
			}
			else
				return (false);
		}

		void		defineResponseHeaderForNonCGI()
		{
			if (!checkFile(fileLocation))
			{
				if (r.rL.method == "DELETE")
					deleteFile();
				else
					contentType = findFileType(fileLocation);
				sL.statusCode = "200";
				sL.reasonPhrase = "OK";
				contentLength = countFileChar(fileLocation);
			}
		}

		void		defineResponseForCGI()
		{
			CGI *cgi = new CGI;
			std::string CGI_PATH = CGIparsing(r, cgi);
			std::cout << "CGI_PATH???" << CGI_PATH << std::endl;
			if (CGI_PATH != "")
			{
				cgi->print_env();
				std::string message = cgi->execute(CGI_PATH, r, _vPfd);
				if (message != "Error")
				{
					size_t position = message.find("\r\n\r\n");
					//size_t position2 = message.find("\r\n\r\n", position + 1);
					sL.statusCode = "200";
					sL.reasonPhrase = "OK";
					contentLength = cgi->get_buffer_size(position + 4);
					body = message;
				}
				else
				{
					sL.statusCode = "500";
					sL.reasonPhrase = "Internal Server Error";
					errorPageLocation(500);
				}
			}
			delete cgi;
		}

		void		deleteFile()
		{                                
			if (unlink(fileLocation.c_str()) == 0) 
				std::cout << "The file is deleted successfully." << std::endl;
			else
			{
				std::cout << "The file is not deleted." << std::endl;
				perror("error");
			}
		}

		int pathType(std::string path, time_t *file_date)
		{
			struct stat buffer;
			struct timezone tz;
			struct timeval t;

			gettimeofday(&t, &tz);
			int exist = stat(path.c_str(), &buffer);
			if (file_date)
				*file_date = buffer.st_mtime + tz.tz_minuteswest * 60;
			if (exist == 0)
			{
				if (S_ISREG(buffer.st_mode))
					return (1);
				else
					return (2);
			}
			else
				return (0);
		}

		int nthOccurrence(const std::string& str, const std::string& findMe, int nth)
		{
			size_t  pos = 0;
			int     cnt = 0;

			while( cnt != nth )
			{
				pos+=1;
				pos = str.find(findMe, pos);
				if ( pos == std::string::npos )
					return -1;
				cnt++;
			}
			return pos;
		}

		std::vector<std::string> split (std::string s, std::string delimiter) {
			std::size_t pos_start = 0, pos_end, delim_len = delimiter.length();
			std::string token;
			std::vector<std::string> res;

			while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
				token = s.substr (pos_start, pos_end - pos_start);
				pos_start = pos_end + delim_len;
				res.push_back (token);
			}

			res.push_back (s.substr (pos_start));
			return res;
		}

		void		foreachUpload()
		{
			std::string upload_body(r.body.begin(), r.body.end());

			std::string delimiter = "--" + r.boundary;
			//std::cout << RED << " delimiter = [" << delimiter << "] " << NC << std::endl;

			std::vector<std::string> splitted_body = split(upload_body, delimiter);

			for (size_t i = 0; i < splitted_body.size(); i++) {
				//std::cout << RED << " [ " << splitted_body[i] << " ] " << NC << std::endl;
				if (splitted_body[i].find("filename") != std::string::npos)
					uploadParsing(splitted_body[i]);
			}

		}

		void		uploadParsing(std::string upload_body)
		{
			upload_filename = upload_body.substr(nthOccurrence(upload_body, "\"", 3));
			upload_filename.erase(0, 1);
			std::size_t pt = upload_filename.find("\"");
			upload_filename.erase(upload_filename.begin() + pt, upload_filename.end());

			std::string content = upload_body.substr(upload_body.find("\r\n\r\n"));
			content.erase(0, 4);
			content.erase(content.end() - 2, content.end());

			uploadFile(upload_filename, "./" + loc.root + "/" +loc.uploadDir, content);
		}

		void		uploadFile(std::string filename, std::string upload_path, std::string content_file)
		{
			int fd = -1;
			std::string path;

			path = upload_path + "/" + filename;
			int type = pathType(path, NULL);
			try
			{
				if (type == 1)
				{
					if ((fd = open(path.c_str(), O_WRONLY | O_TRUNC, 0644)) == -1)
						throw("Error 500");
					write(fd, content_file.c_str(), content_file.length());
					close(fd);
					sL.statusCode = "200";
					sL.reasonPhrase = "OK";
				}
				else if (type == 0)
				{
					if ((fd = open(path.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1)
						throw ("Error 500");
					write(fd, content_file.c_str(), content_file.length());
					close(fd);
					sL.statusCode = "201";
					sL.reasonPhrase = "OK";
				}
				else
				{
					sL.statusCode = "500";
					sL.reasonPhrase = "Internal Server Error";
					errorPageLocation(500);
				}
			}
			catch (std::exception & ex)
			{
				sL.statusCode = "500";
				sL.reasonPhrase = "Internal Server Error";
				errorPageLocation(500);
			}
		}

		std::string CGIparsing(HTTPRequest &request, CGI *cgi)
		{
			std::string FILE_PATH = "", path = fileLocation;
			//std::cout << ">> ext = " << ext << std::endl;
			if (request.isCGI == true)
			{
				std::string ext = std::string(path.substr(path.find_last_of(".")), 0, path.find_last_of("/"));
				cgi->add_variable("SERVER_SOFTWARE", "webserv/1.0");
				cgi->add_variable("SERVER_NAME", "localhost"); // Le nom d'hôte, alias DNS ou adresse IP du serveur.
				cgi->add_variable("GATEWAY_INTERFACE", "CGI/1.1");
				cgi->add_variable("SERVER_PROTOCOL", "HTTP/1.1");
				std::string string;
				std::stringstream ss;
				ss << s.port;
				ss >> string;
				cgi->add_variable("SERVER_PORT", string); // Le port de la requête
				cgi->add_variable("REQUEST_METHOD", request.rL.method);
				//cgi->add_variable("PATH_TRANSLATED", ""); // on laisse tombé ça on copie le path_info
				if (request.rL.requestTarget.find("?") != std::string::npos)
				{
					cgi->add_variable("QUERY_STRING", request.defineQueryString());
					cgi->add_variable("SCRIPT_NAME", request.defineScriptName("?"));
					FILE_PATH = loc.root + request.defineScriptName("?");
				}
				else if (ext.find("/") != std::string::npos)
				{
					cgi->add_variable("PATH_INFO", request.definePathInfo(ext));
					cgi->add_variable("SCRIPT_NAME", request.defineScriptName("/"));
					FILE_PATH = loc.root + request.defineScriptName("/");
				}
				else
				{
					cgi->add_variable("SCRIPT_NAME", request.defineScriptName(""));
					FILE_PATH = loc.root + request.defineScriptName("");
				}
				//cgi->add_variable("REMOTE_HOST", ""); // on laisse vide car DNS inverse désactivé
				//cgi->add_variable("REMOTE_ADDR", ""); // IP du client ??? Demander si on doit vraiment le faire
				cgi->add_variable("AUTH_TYPE", "Basic");
				//cgi->add_variable("REMOTE_USER", "");
				//cgi->add_variable("CONTENT_TYPE", ""); // Récuperer le contenu du POST
				//cgi->add_variable("CONTENT_LENGTH", contentLength.c_str());
				if (request.headerFields.find("Accept") != request.headerFields.end())
					cgi->add_variable("HTTP_ACCEPT", request.headerFields.find("Accept")->second);
				if (request.headerFields.find("Accept-Language") != request.headerFields.end())
					cgi->add_variable("HTTP_ACCEPT_LANGUAGE", request.headerFields.find("Accept-Language")->second);
				if (request.headerFields.find("User-Agent") != request.headerFields.end()) //Ask Raph if test can be performed directly in add_variables
					cgi->add_variable("HTTP_USER_AGENT", request.headerFields.find("User-Agent")->second);
				if (request.headerFields.find("Referer") != request.headerFields.end())
					cgi->add_variable("HTTP_REFERER", request.headerFields.find("Referer")->second);
				if (request.headerFields.find("Cookie") != request.headerFields.end())
					cgi->add_variable("HTTP_COOKIE", request.headerFields.find("Cookie")->second);
				if (checkFile(std::string(loc.root + getenv("SCRIPT_NAME"))))
					return ("");
				return (FILE_PATH);
			}
			else
			{
				std::cout << RED << fileLocation << NC << std::endl;
				cgi->add_variable("QUERY_STRING", "path=./" + std::string(fileLocation, 0, fileLocation.size()));
				return (loc.root + "/cgi-bin/defaultindex.cgi");
			}
		}

		unsigned long long      fileSize(std::string string)
		{
			std::ifstream in_file(string.c_str(), std::ios::binary);
			in_file.seekg(0, std::ios::end);
			unsigned long long file_size = in_file.tellg();
			return (file_size);
		}

		std::string countFileChar(std::string string)
		{
			unsigned long long file_size = fileSize(string);
			std::stringstream ss;
			ss << file_size;
			ss >> string;
			return (string);
		}

		void errorPageLocation(int code)
		{
			std::string codeToString;
			std::stringstream ss;
			ss << code;
			ss >> codeToString;
			size_t i(0);

			contentType = "text/html";
			while (i < s.error.size() && s.error[i].num != code)
				i++;
			if (i < s.error.size() && s.error[i].num == code)
				fileLocation = s.error[i].path;
			else
				fileLocation = loc.root + "/" + codeToString + ".html";
			if (isPathExist(fileLocation))
			{
				contentLength = countFileChar(fileLocation);	
				if (contentLength.size() > 0)
					body = getFileContent(fileLocation);
				contentType = "text/html";
			}
		}

		bool isPathExist(const std::string &s)
		{
			struct stat buffer;
			return (stat (s.c_str(), &buffer) == 0);
		}

		bool checkFilePerm(std::string& path)
		{
			int errno;

			if (r.isCGI)
				if (access(path.c_str(), X_OK) != 0)
					if (errno == 13) //13 = EACCES --> Permission denied.
						return (0);
			if (access(path.c_str(), R_OK) != 0)
				if (errno == 13) //13 = EACCES --> Permission denied.
					return (0);
			return (1);
		}

		bool	checkMethod()
		{
			size_t i(0);
			bool authorized(false);

			if (loc.method.size() == 0)
				return (false);
			while (i < loc.method.size() && authorized == false)
			{
				if (loc.method[i] == r.rL.method)
					authorized = true;
				i++;
			}
			if (authorized == true)
				return (false);
			else
			{
				//std::cout << "ERROR 405\n";
				sL.statusCode = "405";
				sL.reasonPhrase = "Method not allowed";
				errorPageLocation(405);
				return (true);
			}
		}

		bool	checkFile(std::string location)
		{
			if (!isPathExist(location))
			{   
				sL.statusCode = "404";
				sL.reasonPhrase = "Not found";
				errorPageLocation(404);
				return (true);
			}
			if (!checkFilePerm(location))
			{
				sL.statusCode = "403";
				sL.reasonPhrase = "Forbidden";
				errorPageLocation(403);
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
				errorPageLocation(400);
				return (true);
			}
			if (r.rL.method != "GET" && r.rL.method != "POST" && r.rL.method != "DELETE")
			{
				sL.statusCode = "405";
				sL.reasonPhrase = "Method not allowed";
				errorPageLocation(405);
				return (true);
			}
			if (r.rL.httpVersion.find("1.1") == std::string::npos)
			{
				sL.statusCode = "505";
				sL.reasonPhrase = "HTTP version not supported";
				errorPageLocation(505);
				return (true);
			}
			return (false);
		}

		bool	checkClientBodySize()
		{
			if (s.max_body_size > 0)
			{
				if (r.bodySize > s.max_body_size)
				{
					sL.statusCode = "413";
					sL.reasonPhrase = "Request Entity Too Large";
					errorPageLocation(413);
					return (true);
				}
			}
			return (false);
		}

		std::string getFileContent(const std::string& path)
		{
			std::basic_ifstream<char> fin(path.c_str(), std::ios::binary);
			std::ostringstream oss;
			oss << fin.rdbuf();
			std::string data(oss.str());
			return (data);
		}
		std::string findFileType(std::string path)
		{
			std::string str = path.substr(path.find_last_of(".") + 1);

			if (str == "html" || str == "htm" || str == "shtml")
				return ("text/html");
			else if (str == "css")
				return ("text/css");
			else if (str == "xml")
				return ("text/xml");
			else if (str == "gif")
				return ("image/gif");
			else if (str == "jpeg")
				return ("image/jpeg");
			else if (str == "jpg")
				return ("image/jpeg");
			else if (str == "js")
				return ("application/javascript");
			else if (str == "atom")
				return ("application/atom+xml");
			else if (str == "rss")
				return ("application/rss+xml");
			else if (str == "mml")
				return ("text/mathml");
			else if (str == "txt")
				return ("text/plain");
			else if (str == "jad")
				return ("text/vnd.sun.j2me.app-descriptor");
			else if (str == "wml")
				return ("text/vnd.wap.wml");
			else if (str == "htc")
				return ("text/text/x-component");
			else if (str == "png")
				return ("image/png");
			else if (str == "svg" || str == "svgz")
				return ("image/svg+xml");
			else if (str == "tif" || str == "tiff")
				return ("image/tiff");
			else if (str == "wbmp")
				return ("image/vnd.wap.wbmp");
			else if (str == "webp")
				return ("image/webp");
			else if (str == "ico")
				return ("image/x-icon");
			else if (str == "jng")
				return ("image/x-jng");
			else if (str == "bmp")
				return ("image/x-ms-bmp");
			else if (str == "woff")
				return ("font/woff");
			else if (str == "woff2")
				return ("font/woff2");
			else if (str == "jar" || str == "war" || str == "ear")
				return ("application/java-archive");
			else if (str == "json")
				return ("application/json");
			else if (str == "hqx")
				return ("application/mac-binhex40");
			else if (str == "doc")
				return ("application/msword");
			else if (str == "pdf")
				return ("application/pdf");
			else if (str == "ps" || str == "eps" || str == "ai")
				return ("application/postscript");
			else if (str == "rtf")
				return ("application/rtf");
			else if (str == "m3u8")
				return ("application/vnd.apple.mpegurl");
			else if (str == "kml")
				return ("application/vnd.google-earth.kml+xml");
			else if (str == "kmz")
				return ("application/vnd.google-earth.kmz");
			else if (str == "xls")
				return ("application/vnd.ms-excel");
			else if (str == "eot")
				return ("application/vnd.ms-fontobject");
			else if (str == "ppt")
				return ("application/vnd.ms-powerpoint");
			else if (str == "odg")
				return ("application/vnd.oasis.opendocument.graphics");
			else if (str == "odp")
				return ("application/vnd.oasis.opendocument.presentation");
			else if (str == "ods")
				return ("application/vnd.oasis.opendocument.spreadsheet");
			else if (str == "odt")
				return ("application/vnd.oasis.opendocument.text");
			else if (str == "pptx")
				return ("application/vnd.openxmlformats-officedocument.presentationml.presentation");
			else if (str == "xlsx")
				return ("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
			else if (str == "docx")
				return ("application/vnd.openxmlformats-officedocument.wordprocessingml.document");
			else if (str == "bin" || str == "exe" || str == "ddl" || str == "deb" || str == "dmg"
					|| str == "iso" || str == "img" || str == "msi" || str == "msp" || str == "msm")
				return ("application/octet-stream");
			else if (str == "wmlc")
				return ("application/vnd.wap.wmlc");
			else if (str == "7z")
				return ("application/x-7z-compressed");
			else if (str == "cco")
				return ("application/x-cocoa");
			else if (str == "jardiff")
				return ("application/x-java-archive-diff");
			else if (str == "jnlp")
				return ("application/x-java-jnlp-file");
			else if (str == "run")
				return ("application/x-makeself");
			else if (str == "pl" || str == "pm")
				return ("application/x-perl");
			else if (str == "prc" || str == "pdb")
				return ("application/x-pilot");
			else if (str == "rar")
				return ("application/x-rar-compressed");
			else if (str == "rpm")
				return ("application/x-redhat-package-manager");
			else if (str == "sea")
				return ("application/x-sea");
			else if (str == "swf")
				return ("application/x-shockwave-flash");
			else if (str == "sit")
				return ("application/x-stuffit");
			else if (str == "tcl" || str == "tk")
				return ("application/x-tcl");
			else if (str == "der" || str == "pem" || str == "crt")
				return ("application/x-x509-ca-cert");
			else if (str == "xpi")
				return ("application/x-xpinstall");
			else if (str == "xhtml")
				return ("application/xhtml+xml");
			else if (str == "xspf")
				return ("application/xspf+xml");
			else if (str == "zip")
				return ("application/zip");
			else if (str == "mid" || str == "midi" || str == "kar")
				return ("audio/midi");
			else if (str == "mp3")
				return ("audio/mpeg");
			else if (str == "ogg")
				return ("audio/ogg");
			else if (str == "m4a")
				return ("audio/x-m4a");
			else if (str == "ra")
				return ("audio/x-realaudio");
			else if (str == "3gpp" || str == "3gp")
				return ("video/3gpp");
			else if (str == "ts")
				return ("video/mp2t");
			else if (str == "mp4")
				return ("video/mp4");
			else if (str == "mpeg" || str == "mpg")
				return ("video/mpeg");
			else if (str == "mov")
				return ("video/quicktime");
			else if (str == "webm")
				return ("video/webm");
			else if (str == "flv")
				return ("video/x-flv");
			else if (str == "m4v")
				return ("video/x-m4v");
			else if (str == "mng")
				return ("video/x-mng");
			else if (str == "asx" || str == "asf")
				return ("video/x-ms-asf");
			else if (str == "wmv")
				return ("video/x-ms-wmv");
			else if (str == "avi")
				return ("video/x-msvideo");
			return ("text/plain");
		}

		HTTPRequest			r;
		struct server		s;
		struct location		loc;
		bool				invalidHost;
		std::string			fileLocation;
		std::string			contentType;
		std::string			contentLength;
		std::string			body;
		std::string			redirectionLocation;
		std::string			upload_filename;
		PollFd					&_vPfd;
	public:
		std::string	resp;
		struct statusLine   sL;
};

#endif
