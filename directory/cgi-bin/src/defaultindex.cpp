/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaultindex.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 17:03:35 by rbourgea          #+#    #+#             */
/*   Updated: 2021/11/18 15:38:39 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <iostream>

int main(void)
{
    char path[80] = "";
    char *data = getenv("QUERY_STRING");
    if (!data)
        data = "";
    DIR *d;
    struct dirent *dir;
    if (sscanf(data, "path=%s", &path) < 0)
        d = opendir(".");
    else
        d = opendir(strcpy("directory", path));
    std::cout << "Content-type:text/html\r\n\r\n";
    if (d)
    {
        std::cout << "<h1>Index Of:</h1><br>" << std::endl;
        while ((dir = readdir(d)) != NULL)
        {
            std::cout << "<a href=" << path << "/" << dir->d_name << ">" << dir->d_name << "</a>\n<br><br>";
        }
        closedir(d);
    }
    return (0);
}
