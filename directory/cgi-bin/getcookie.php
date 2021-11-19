#!/usr/bin/php
<?php echo "Content-type:text/html\r\n\r\n"; ?>

<html>
  <head>
    <title>Cookie get !</title>
  </head>
  <body>
    <p>Hello, <?php echo $_COOKIE['LOGGED_USER']; ?> !</p>
  </body>
</html>
