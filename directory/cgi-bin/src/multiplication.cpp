/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiplication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 15:43:36 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/10 13:07:24 by rbourgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
     int a=0, b=0;

     printf("Content-Type: text/html;\r\n\r\n");

     char *data = getenv("QUERY_STRING"); // Les variables
     if(data == NULL)
     {
          printf("<p>Erreur dans le script !</p>");
     }
     else
     {
          if(sscanf(data, "a=%d&b=%d", &a, &b) != 2) // On récupère les deux chiffres a et b
               printf("<p>Vous devez passer des chiffres en données !");
          else
               printf("<p>%dx%d = %d ! :)</p>", a, b, a*b);
     }
     return 0;
}