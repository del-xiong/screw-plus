                    [ PHP Screw - User's Manual ver1.5 ]

1. What's SCREW?

 PHP Screw is a PHP script encryption tool. When you are developing a
 commercial package using PHP, the script can be distributed as encrypted up
 until just before execution. This preserves your intellectual property.


2. Outline.

 The outline of PHP Screw is as follows.

 a. Encipher a PHP script with the encryption tool (screw).

 b. At the time of execution of a PHP script, the decryptor php_screw.so is
    executed as PHP-Extension, just before the PHP script is handed to the
    Zend-Compiler.

   * In fact what is necessary is just to add the description about php.screw
     to php.ini. A PHP script programmer does not need to be conscious of
     decrypting process.
     Moreover it is possible for you to intermingle an enciphered script file
     and an unenciphered one.

   * The encryption logic in the encryption tool (screw) and the decryption
     logic in the decryptor (php_screw), can be cutomized easily.

   * The normal purpose code and decryption logic included, can be customized
     by only changing the encryption SEED key.
     Although it is easy to cusomize the encryption, by the encryption SEED, it
     does NOT mean, that the PHP scripts can be decrypted by others easily.


3. Requirement

 (1) PHP5.x
     NOTICE: PHP must be compiled with the "--with-zlib" option.
     Check that PHP has zlib compiled in it with the PHP script:
     "<? gzopen(); ?>"
     If PHP knows about the function you can happily proceed.

 (2) UNIX like OS (LINUX, FreeBSD, etc. are included) 


4. Installation.

 (1) Unpack php_screw-1.5.tgz.

 (2) Execute the following command with directory php_screw-1.5

       phpize
       ./configure

 (3) Customize encryption / decryption

       vi my_screw.h

       -- Please change the encryption SEED key (pm9screw_mycryptkey) into the
          values according to what you like.
          The encryption will be harder to break, if you add more values to the
	  encryption SEED array. However, the size of the SEED is unrelated to
	  the time of the decrypt processing.

       *  If you can read and understand the source code, to modify an original
          encryption logic will be possible. But in general, this should not
	  be necessary.

       OPTIONAL: Encrypted scripts get a stamp added to the beginning of the
		 file. If you like, you may change this stamp defined by
		 PM9SCREW and PM9SCREW_LEN in php_screw.h. PM9SCREW_LEN must
		 be less than or equal to the size of PM9SCREW.

 (4) Compile the decryptor

       make

 (5) Copy modules/php_screw.so into an extension directory.

       ex) cp modules/php_screw.so /usr/lib/php/modules/php_screw.so

 (6) Add the next line to php.ini.

       extension=php_screw.so

 (7) Restart the httpd daemon.

 (8) Compile the encryption tool

       cd tools
       make

 (9) Copy the encryptor ("screw") into an appropriate directory.


5. User's manual.

 (1) Encrypting a PHP script:
     Please execute the following command.

        screw "Path to the PHP script to be encrypted"

     The above line creates the script file enciphered by the same name.
     Moreover, a backup is created in the same directory by the name of script
     file name .screw.
     WARNING: When you perform packaging, do NOT forget remove the .screw file.
                             
 (2) Executing a PHP script.
     If the installation of php_screw.so went OK, encrypted PHP scripts can now
     be copied to an appropriate directory and executed as if it was
     unencrypted. That is: The encryption is transparent for the
     user.


6. History

 2002/04/25 PHP Screw ver1.0
 2002/05/21 PHP Screw ver1.1 -- Fixed include_path problem.
 2002/08/02 PHP Screw ver1.2 -- Changed encryption algorithm
 2004/01/11 PHP Screw ver1.3 -- Changed to BSD License(revised), Correspondence to php-4.3.x
 2007/10/11 PHP Screw ver1.5 -- Correspondence to PHP5.x


7. Copyright
 (C) 2007 Kunimasa Noda/PM9.com, Inc.
 contact:  http://www.pm9.com,  kuni@pm9.com

