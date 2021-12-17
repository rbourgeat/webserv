#!/bin/bash

telnet localhost 8080;
sleep 5;
echo "POST /cgi-bin/postdata.cgi HTTP/1.1
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip,
Accept-Language: en-US,en;q=0.9
Cache-Control: max-age=0
Connection: keep-alive
Content-Length: 28
Content-Type: application/x-www-form-urlencoded
Host: localhost:8081
Origin: http://localhost:8081
Referer: http://localhost:8081/
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: same-origin
Sec-Fetch-User: ?1
Sec-Fetch-Dest:
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0
sec-ch-ua: 
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: macOS

data=ecole+42&submit=Envoyer";
