dnl config.m4 for extension php_screw_plus
dnl don't forget to call PHP_EXTENSION(php_screw_plus)
dnl If your extension references something external, use with:

PHP_ARG_WITH(php_screw_plus, for php_screw_plus support,
dnl Make sure that the comment is aligned:
[  --with-php_screw_plus             Include php_screw_plus support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(php_screw_plus, whether to enable php_screw_plus support,
dnl Make sure that the comment is aligned:
[  --enable-php_screw_plus           Enable php_screw_plus support])

if test "$PHP_php_screw" != "no"; then
  dnl Action..
  PHP_NEW_EXTENSION(php_screw_plus, php_screw_plus.c, $ext_shared)
fi
