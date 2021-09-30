/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 16:33:46 by rbourgea          #+#    #+#             */
/*   Updated: 2021/09/30 16:55:48 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/socket.h>
// DOC: https://pubs.opengroup.org/onlinepubs/007908775/xns/syssocket.h.html
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <cstring>
# include <iostream>
# include <sys/stat.h>
# include <fstream>
# include <sstream>
# include <vector>

# define PORT 8080

// const char* parsing(char* buffer);

std::vector<unsigned char> parsing(std::vector<unsigned char> buffer);