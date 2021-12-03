/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiplication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgeat <rbourgeat@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 15:43:36 by rbourgea          #+#    #+#             */
/*   Updated: 2021/12/03 15:34:44 by rbourgeat        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h> // Ne pas oublier pour getenv()

void haut(char *);
void bas();

int main(void)
{
     int a=0, b=0;

     printf("Content-Type: text/html;\r\n\r\n");
     haut("Ma page en C !");

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
     bas();
     return 0;
}

void haut(char *title) {
     printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"fr\" >\n\t<head>");
     printf("\t\t<title>%s</title>", title);
     printf("\t\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n\t</head>\n\t<body>");
}

void bas() {
     printf("\t</body>\n</html>");
}