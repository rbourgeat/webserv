/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaultindex.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 17:03:35 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/07 15:09:51 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <iostream>
#include <string>

int main(void)
{
    char *path;
    char *data = getenv("QUERY_STRING");
    DIR *d;
    struct dirent *dir;
    if (data != NULL)
        if (sscanf(data, "path=%s", path) < 0)
            d = opendir("./directory");
        else
        {
            std::string str1(path);
            std::string str2 = "./directory/" + str1;
            d = opendir(str2.c_str());
        }
    else
        d = opendir("./directory");
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
