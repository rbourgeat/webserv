#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "webserv.hpp"
# include "CGI.hpp"
# include "serverAndClient.hpp"

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

		std::string	defineResponse()
		{
			if (s.redi.path.size() > 0)
				redirectClient();
			else if (!checkRequest() && !checkClientBodySize())
			{
				if (s.root.size() > 1)
					fileLocation = s.root;
				else
					fileLocation = "directory";
				fileLocation+= r.rL.requestTarget;
				if (!r.isCGI)
				{
					defineResponseHeaderForNonCGI();
					if (contentLength.size() > 0)
						body = getFileContent(fileLocation);
				}
				else
					defineResponseForCGI();
			}
			aggregateResponse();
			return (resp);
		}

	private:

		void	redirectClient()
		{
			sL.statusCode = s.redi.num;
			if (s.redi.num == 301)
				sL.reasonPhrase = "Moved permanently";
			redirectionLocation = s.redi.path;
		}
		void	aggregateResponse()
		{
			resp+= sL.httpVersion + " " + sL.statusCode + " " + sL.reasonPhrase + "\r\n";
			if (r.rL.method != "DELETE" && redirectionLocation.size() == 0)
			{
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

		void		defineResponseHeaderForNonCGI()
		{                        
			if (!checkFile())
			{
				if (r.rL.method == "DELETE")
					deleteFile();
				else
				{
					if (fileLocation == (s.root + "/")) //REDIRECTION TO INDEX.HTML in case we're looking in root directory => TO DEFINE IN CONFIG FILE
						fileLocation+= "index.html";
					contentType = findFileType(fileLocation);
				}
				sL.statusCode = "200";
				sL.reasonPhrase = "OK";
				contentLength = countFileChar(fileLocation);
				setenv("CONTENT_LENGTH", contentLength.c_str(), 1); //Ask Raph if this is still necessary
			}
		}

		void		defineResponseForCGI()
		{
			CGI *cgi = new CGI;
			std::string CGI_PATH = CGIparsing(r, cgi);
			cgi->print_env();
			std::string message = cgi->execute(CGI_PATH, r);
			std::cout << "test" << std::endl;
			if (message != "Error")
			{
				size_t position = message.find("\n");
				size_t position2 = message.find("\n", position + 1);
				sL.statusCode = "200";
				sL.reasonPhrase = "OK";
				contentLength = cgi->get_buffer_size(position2);
				setenv("CONTENT_LENGTH", contentLength.c_str(), 1);
				body = message;
			}
			else
			{
				sL.statusCode = "500";
				sL.reasonPhrase = "Internal Server Error";
				errorPageLocation(500);
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

		std::string CGIparsing(HTTPRequest &request, CGI *cgi)
		{
			std::string FILE_PATH = "";
			if (request.isCGI == true)
			{
				cgi->add_variable("SERVER_SOFTWARE", "webserv/1.0");
				cgi->add_variable("GATEWAY_INTERFACE", "CGI/1.1");
				cgi->add_variable("SERVER_PROTOCOL", "HTTP/1.1");
				cgi->add_variable("AUTH_TYPE", "Basic");
				cgi->add_variable("QUERY_STRING", request.defineQueryString());
				cgi->add_variable("PATH_INFO", "");
				cgi->add_variable("SCRIPT_NAME", request.defineScriptName());
				FILE_PATH = "directory" + request.defineScriptName();
				if (request.headerFields.find("User-Agent") != request.headerFields.end()) //Ask Raph if test can be performed directly in add_variables
					cgi->add_variable("HTTP_USER_AGENT", request.headerFields.find("User-Agent")->second);
				if (request.headerFields.find("Referer") != request.headerFields.end())
					cgi->add_variable("HTTP_REFERER", request.headerFields.find("Referer")->second);
				if (request.headerFields.find("Accept") != request.headerFields.end())
					cgi->add_variable("HTTP_ACCEPT", request.headerFields.find("Accept")->second);
				if (request.headerFields.find("Accept-Language") != request.headerFields.end())
					cgi->add_variable("HTTP_ACCEPT_LANGUAGE", request.headerFields.find("Accept-Language")->second);
			}
			return (FILE_PATH);
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
				fileLocation = "directory/" + s.error[i].path;
			else
				fileLocation = "directory/" + codeToString + ".html";
			contentLength = countFileChar(fileLocation);	
			if (contentLength.size() > 0)
				body = getFileContent(fileLocation);
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
				errorPageLocation(404);
				return (true);
			}
			if (!checkFilePerm(fileLocation))
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
		struct statusLine   sL;
		std::string			fileLocation;
		std::string			contentType;
		std::string			contentLength;
		std::string			body;
		std::string			redirectionLocation;

	public:
		std::string			resp;
};

#endif
