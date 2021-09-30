/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classWebserv.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 16:33:46 by rbourgea          #+#    #+#             */
/*   Updated: 2021/09/30 10:14:20 by dgoudet          ###   ########.fr       */
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

# define PORT 8081

const char* parsing(char* buffer);
