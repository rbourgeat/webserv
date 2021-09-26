/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 16:30:12 by rbourgea          #+#    #+#             */
/*   Updated: 2021/09/26 16:05:55 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "classWebserv.hpp"

std::string findTypeofFile(std::string path)
{
	std::string str = path.substr(path.find_last_of(".") + 1);

	if (str == "html")
		return ("text/html");
	else if (str == "htm")
		return ("text/html");
	else if (str == "shtml")
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
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  return content;
}

bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

std::string countFileChar(std::string string)
{
	std::ifstream in_file(string, std::ios::binary);
	in_file.seekg(0, std::ios::end);
	int file_size = in_file.tellg();
	std::stringstream ss;
	ss << file_size;
	ss >> string;
    return (string);
};

const char* parsing(char *buffer)
{
	int j = 0;
	std::string location = "directory";
	std::string s(buffer);
	std::string delimiter = " ";
	std::string message;
	std::string error;
	std::string method = "";

	size_t pos = 0;
	std::string token;
	// remove all newlines
	while ( s.find ("\r\n") != std::string::npos )
	{
		s.erase ( s.find ("\r\n"), 2 );
	}

	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		if (j == 0)
			method = token;

		if (j == 1)
			location += token;
		s.erase(0, pos + delimiter.length());
		j++;
	}
	if (method == "")
		method = s;

	std::cout << "[" << method << "]" << std::endl;
	
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		message += "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html";
		location = "directory/405.html";
	}
	else if ((method == "GET" || method == "POST" || method == "DELETE")
				&& location == "directory")
	{
		message += "HTTP/1.1 400 Method Not Allowed\r\nContent-Type: text/html";
		location = "directory/400.html";
	}
	else if (!IsPathExist(location))
	{
		message += "HTTP/1.1 404 Not Found\r\nContent-Type: text/html";
		location = "directory/404.html";
	}
	// else if (s.find("Content-Length") == std::string::npos)
	// {
	// 	message += "HTTP/1.1 411 Length Required";
	// }
	else if (location == "directory/")
	{
		message += "HTTP/1.1 200 OK\r\nContent-Type: text/html";
		location = "directory/index.html";
	}
	else
	{
		message += "HTTP/1.1 200 OK\r\nContent-Type: ";
		message += findTypeofFile(location);
	}

	message += "\r\nContent-Length: ";
	message += countFileChar(location);
	message += "\r\n\r\n" + getFileContent(location);

	const char* c = message.c_str();
	std::cout << "[c: " << c << "]" << std::endl;
	return (c);
}

