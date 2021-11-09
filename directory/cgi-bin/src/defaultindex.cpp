/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaultindex.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 17:03:35 by rbourgea          #+#    #+#             */
/*   Updated: 2021/11/08 17:46:24 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <iostream>

int main(void)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    std::cout << "Content-type:text/html\r\n\r\n";
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            std::cout << dir->d_name << "\n<br><br>";
        }
        closedir(d);
    }
    return (0);
}
