#!/usr/bin/php
<?php echo "Content-type:text/html\r\n\r\n"; ?>
<?php
// retenir l'email de la personne connectée pendant 1 an
setcookie(
    'LOGGED_USER',
    'Norminet',
    [
        'expires' => time() + 365*24*3600,
        'secure' => true,
        'httponly' => true,
    ]
);
?>

<html>
  <head>
    <title>Cookie set !</title>
  </head>
  <body>
    <p><?php echo "Cookie set !"; ?></p>
  </body>
</html>