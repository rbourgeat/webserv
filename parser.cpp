/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 16:30:12 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/06 14:23:34 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.hpp"
# include "serverAndClient.hpp"
# include "CGI.hpp"
# include <sys/time.h>

std::string findTypeofFile(std::string path)
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

std::string getFileContent(const std::string& path)
{
	std::basic_ifstream<char> fin(path.c_str(), std::ios::binary);
	std::ostringstream oss;
	oss << fin.rdbuf();
	std::string data(oss.str());
	return (data);
}

bool IsPathExist(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}


bool CheckFilePerm(std::string& path)
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

unsigned long long		fileSize(std::string string)
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
};

// void print_CGIenv()
// {
// 	std::cout << "\n---------------------------" << std::endl;
// 	std::cout << "| CGI Environment Variables:" << std::endl;
// 	std::cout << "---------------------------" << std::endl;
// 	std::cout << "| SERVER_SOFTWARE = " << getenv("SERVER_SOFTWARE") << std::endl;
// 	std::cout << "| SERVER_NAME = " << "???" << std::endl;
// 	std::cout << "| GATEWAY_INTERFACE = " << getenv("GATEWAY_INTERFACE") << std::endl;
// 	std::cout << "|" << std::endl;
// 	std::cout << "| SERVER_PROTOCOL = " << getenv("SERVER_PROTOCOL") << std::endl;
// 	std::cout << "| SERVER_PORT = " << "???" << std::endl;
// 	std::cout << "| REQUEST_METHOD = " << getenv("REQUEST_METHOD") << std::endl;
// 	std::cout << "| PATH_INFO = " << getenv("PATH_INFO") << std::endl;
// 	std::cout << "| PATH_TRANSLATED = " << "???" << std::endl;
// 	std::cout << "| SCRIPT_NAME = " << getenv("SCRIPT_NAME") << std::endl;
// 	std::cout << "| QUERY_STRING = " << getenv("QUERY_STRING") << std::endl;
// 	std::cout << "| REMOTE_HOST = " << "???" << std::endl;
// 	std::cout << "| REMOTE_ADDR = " << "???" << std::endl;
// 	std::cout << "| AUTH_TYPE = " << getenv("AUTH_TYPE") << std::endl;
// 	std::cout << "| REMOTE_USER = " << "???" << std::endl;
// 	std::cout << "| REMOTE_IDENT = " << "???" << std::endl;
// 	std::cout << "| CONTENT_TYPE = " << "???" << std::endl;
// 	std::cout << "| CONTENT_LENGTH = " << getenv("CONTENT_LENGTH") << std::endl;
// 	std::cout << "|" << std::endl;
// 	std::cout << "| HTTP_ACCEPT = " << getenv("HTTP_ACCEPT") << std::endl;
// 	std::cout << "| HTTP_ACCEPT_LANGUAGE = " << getenv("HTTP_ACCEPT_LANGUAGE") << std::endl;
// 	std::cout << "| HTTP_USER_AGENT = " << getenv("HTTP_USER_AGENT") << std::endl;
// 	std::cout << "| HTTP_COOKIE = " << "???" << std::endl;
// 	std::cout << "| HTTP_REFERER = " << getenv("HTTP_REFERER") << std::endl;
// 	std::cout << "---------------------------" << std::endl;
// }

std::string CGIparsing(HTTPRequest &request, std::vector<unsigned char> buffer, CGI *cgi)
{
	(void)buffer;
	std::string FILE_PATH = "";
	if (request.isCGI == true)
	{
		//std::cout << "YOOOU " << request.defineQueryString() << std::endl;
		//std::cout << "YAAAAA " << request.defineScriptName() << std::endl;
		cgi->add_variable("SERVER_SOFTWARE", "webserv/1.0");
    	cgi->add_variable("GATEWAY_INTERFACE", "CGI/1.1");
    	cgi->add_variable("SERVER_PROTOCOL", "HTTP/1.1");
    	cgi->add_variable("AUTH_TYPE", "Basic");
		cgi->add_variable("QUERY_STRING", request.defineQueryString());
		cgi->add_variable("PATH_INFO", "");
		cgi->add_variable("SCRIPT_NAME", request.defineScriptName());
		FILE_PATH = "directory" + request.defineScriptName();
		if (request.headerFields.find("User-Agent") != request.headerFields.end())
			cgi->add_variable("HTTP_USER_AGENT", request.headerFields.find("User-Agent")->second);
		if (request.headerFields.find("Referer") != request.headerFields.end())
			cgi->add_variable("HTTP_REFERER", request.headerFields.find("Referer")->second);
		if (request.headerFields.find("Accept") != request.headerFields.end())
			cgi->add_variable("HTTP_ACCEPT", request.headerFields.find("Accept")->second);
		if (request.headerFields.find("Accept-Language") != request.headerFields.end())
			cgi->add_variable("HTTP_ACCEPT_LANGUAGE", request.headerFields.find("Accept-Language")->second);
	}
	/*
	std::vector<char> tmp;
	std::string FILE_PATH = "";
	int i = 0;

	while ((size_t)i < buffer.size())
	{
		tmp.push_back(buffer[i]);
		i++;
	}
	std::string s_tmp(tmp.begin(), tmp.end());

	cgi->add_variable("SERVER_SOFTWARE", "webserv/1.0");	// A changer (get celui du conf)
	cgi->add_variable("SERVER_NAME", "webserv");			// same
	cgi->add_variable("GATEWAY_INTERFACE", "CGI/1.1");
	cgi->add_variable("SERVER_PROTOCOL", "HTTP/1.1");
	cgi->add_variable("SERVER_PROTOCOL", "8080");			// remplacer
	cgi->add_variable("AUTH_TYPE", "Basic");

	std::istringstream buffer2(s_tmp);
	std::string line;
	size_t first;

	while (getline(buffer2, line, '\n'))
	{
		if (line.empty() || line == "\r")
			break;
		if (line[line.size() - 1] == '\r')*//*(line.back() == '\r') back() is a c++11 function*/
		/*	line.resize(line.size() - 1);

		// demander si le cgi doit fonctionner en dehors du /cgi-bin/
		first = line.find("GET /cgi-bin/");
		if (first != std::string::npos && (line.find(".cgi") != std::string::npos)) //MODIF BY CONFIG
		{
			line.erase(first, 13);
			first = line.find(" HTTP/");
			line.erase(first, 9);
			first = line.find("?");
			if (first != std::string::npos)
			{
				cgi->add_variable("QUERY_STRING", line.substr(first + 1));
				cgi->add_variable("PATH_INFO", "");
				std::string nscript = "/cgi-bin/" + line.replace(line.find("?"), std::string::npos, "");
				cgi->add_variable("SCRIPT_NAME", nscript);
				FILE_PATH = "directory" + nscript;
			}
			else if (line.find(".cgi/") != std::string::npos) //CONFIG EXTENTION CGI
			{
				first = line.find(".cgi/");
				cgi->add_variable("QUERY_STRING", "");
				cgi->add_variable("PATH_INFO", line.substr(first + 5));
				std::string nscript = "/cgi-bin/" + line.replace(line.find("/"), std::string::npos, "");
				cgi->add_variable("SCRIPT_NAME", nscript);
				FILE_PATH = "directory" + nscript;
			}
			else
			{
				cgi->add_variable("QUERY_STRING", "");
				cgi->add_variable("PATH_INFO", "");
				std::string nscript = "/cgi-bin/" + line;
				cgi->add_variable("SCRIPT_NAME", nscript);
				FILE_PATH = "directory" + nscript;
			}
		}

		first = line.find("User-Agent: ");
		if (first != std::string::npos)
			cgi->add_variable("HTTP_USER_AGENT", line.substr(first + 12));

		first = line.find("Referer: ");
		if (first != std::string::npos)
			cgi->add_variable("HTTP_REFERER", line.substr(first + 9));

		first = line.find("Accept: ");
		if (first != std::string::npos)
			cgi->add_variable("HTTP_ACCEPT", line.substr(first + 8));

		first = line.find("Accept-Language: ");
		if (first != std::string::npos)
			cgi->add_variable("HTTP_ACCEPT_LANGUAGE", line.substr(first + 17));

	}*/
	return (FILE_PATH);
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

std::string	errorPageLocation(int code, struct server s)
{
	std::string location, codeToString;
	std::stringstream ss;
	ss << code;
	ss >> codeToString;
	size_t i(0);

	while (i < s.error.size() && s.error[i].num != code)
		i++;
	if (i < s.error.size() && s.error[i].num == code)
		location = "directory/" + s.error[i].path;
	else
		location = "directory/" + codeToString + ".html";
	return (location);

}


std::vector<unsigned char> parsing(HTTPRequest &request, std::vector<unsigned char> buffer, struct server s)
{
	std::string location, s_tmp_previous;
	std::vector<char> tmp;
	/*unsigned long long max; //maybe problem comes from size_t max value
	if (s.max_body_size > 0)
		max = s.max_body_size;
	else
		max = 1000000; //1 Megabyte (default for nginx)*/
	if (s.root.size() > 0)
		location = s.root;
	else
		location = "directory";
	
	std::cout << "location before: " << location << std::endl;
	

	std::string rep;
	location += request.rL.requestTarget;
	std::cout << "location: " << location << std::endl;	
	if (request.isCGI == false)
	{
		//std::cout << "********************NO CGI*************************\n";
		if (request.rL.method != "GET" && request.rL.method != "POST" && request.rL.method != "DELETE") //OK
		{
			rep = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html";
			location = errorPageLocation(405, s);
		}
		/*else if ((METHOD == "GET" || METHOD == "POST" || METHOD == "DELETE")
				&& location == "directory")
		{
			rep = "HTTP/1.1 400 Method Not Allowed\r\nContent-Type: text/html";
			location = errorPageLocation(400, s);
		}*/
		else if (!IsPathExist(location)) //OK
		{
			rep = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html";
			location = errorPageLocation(404, s);
		}
		// else if (s.find("Content-Length") == std::string::npos)
		// {
		// 	message += "HTTP/1.1 411 Length Required";
		// }
		else if (location == (s.root + "/")) //OK
		{
			rep = "HTTP/1.1 200 OK\r\nContent-Type: text/html";
			location += "index.html";
		}
		else if (request.rL.httpVersion.find("1.1") == std::string::npos) //OK
		{
			rep = "HTTP/1.1 505 HTTP Version not supported\r\nContent-Type: text/html";
			location = errorPageLocation(505, s);
		}
		else if (!CheckFilePerm(location)) //OK
		{
			rep = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html";
			location = errorPageLocation(403, s);
		}
		else if (0 == 1) // Trouver comment parser ?!
		{
			int fd = -1;
			std::string path, filename; // Récupérer le nom du fichier
			std::string upload_path; // Chemin par défaut pour upload
			const std::string content_file; // Trouver le moyen de recup le contenu du fichier

			path = upload_path + "/" + filename;
			int type = pathType(path, NULL);
			try
			{
				if (type == 1)
				{
					if ((fd = open(path.c_str(), O_WRONLY | O_TRUNC, 0644)) == -1)
						throw(location = errorPageLocation(500, s));
					write(fd, content_file.c_str(), content_file.length());
					close(fd);
					rep = "HTTP/1.1 200 OK\r\nContent-Type: text/html";
					location += "index.html";
				}
				else if (type == 0)
				{
					if ((fd = open(path.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0644)) == -1)
						throw (location = errorPageLocation(500, s));
					write(fd, content_file.c_str(), content_file.length());
					close(fd);
					rep = "HTTP/1.1 201 OK\r\nContent-Type: text/html";
					location += "index.html";
				}
				else
					location = errorPageLocation(500, s);
			}
			catch (std::exception & ex)
			{
				location = errorPageLocation(500, s);
			}

		}
		else if (request.rL.method == "DELETE") //OK
		{
			if (unlink(location.c_str()) == 0) {
				std::cout << "The file is deleted successfully." << std::endl;
			} else {
				std::cout << "The file is not deleted." << std::endl;
				perror("error");
			}
			rep = "HTTP/1.1 200 OK\r\n";
		}
		else //OK
		{
			rep = "HTTP/1.1 200 OK\r\nContent-Type: ";
			rep += findTypeofFile(location);
		}
		
		if (s.max_body_size > 0)
		{
			if (fileSize(location) > s.max_body_size)
			{
				std::cout << "file size: " << fileSize(location) << std::endl;
				rep = "HTTP/1.1 413 Request Entity Too Large\r\nContent-Type: text/html";
				location = errorPageLocation(505, s); //create a real page for that
			}
		}

		if (request.rL.method != "DELETE") //OK
		{
			rep += "\r\nContent-Length: ";
			rep += countFileChar(location);
			setenv("CONTENT_LENGTH", countFileChar(location).c_str(), 1);
			rep += "\r\n\r\n" + getFileContent(location);
		}
		std::cout << "LOCATION="<< location << std::endl;
	}
	else //OK
	{
		CGI *cgi = new CGI; 
		std::string CGI_PATH = CGIparsing(request, buffer, cgi);
		cgi->print_env();
		std::string message = cgi->execute(CGI_PATH, request);
		if (message != "Error")
		{
			size_t position = message.find("\n");
			size_t position2 = message.find("\n", position + 1);
			rep += "HTTP/1.1 200 OK\r\n";
			rep += "Content-Length: ";
			rep += cgi->get_buffer_size(position2);
			setenv("CONTENT_LENGTH", cgi->get_buffer_size(position2).c_str(), 1);
			rep += "\r\n" + message;
		}
		else
		{
			rep = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html";
			location = errorPageLocation(500, s);
			rep += "\r\nContent-Length: ";
			rep += countFileChar(location);
			rep += "\r\n\r\n" + getFileContent(location);
		}
		delete cgi;
	}

	std::vector<unsigned char> response(rep.begin(), rep.end());
	return (response);
}
