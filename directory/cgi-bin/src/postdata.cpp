/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postdata.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbourgea <rbourgea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 16:22:58 by rbourgea          #+#    #+#             */
/*   Updated: 2021/11/25 19:42:26 by dgoudet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

void haut(char *);
void bas();

int main(void)
{
     char montexte[80] = "";
     printf("Content-Type: text/html\r\n\r\n");
     haut("Ma page en C !");
     if(fscanf(stdin, "data=%80[^&]s", montexte) < 0)
          printf("<p>Le formulaire a été mal rempli !</p>");
     else
          printf("Voici ce que vous avez écrit : %s", montexte);
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
