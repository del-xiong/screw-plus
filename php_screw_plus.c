/*
 * php_screw
 * (C) 2007, Kunimasa Noda/PM9.com, Inc. <http://www.pm9.com,  kuni@pm9.com>
 * see file LICENSE for license details
 */

#include "php.h"
#include "php_ini.h"
#include "ext/standard/file.h"
#include "ext/standard/info.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
 #include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "php_screw_plus.h"
#include "md5.h"
#include "aes.c"
#include "aes_crypt.c"

PHP_MINIT_FUNCTION(php_screw_plus);
PHP_MSHUTDOWN_FUNCTION(php_screw_plus);
PHP_MINFO_FUNCTION(php_screw_plus);

FILE *pm9screw_ext_fopen(FILE *fp)
{
  struct  stat  stat_buf;
  char  *datap, *newdatap;
  char lenBuf[16];
  int datalen, newdatalen=0;
  int i;
  uint8_t enTag[16];
  uint8_t key[64];
  memset(key, 0, sizeof(key));
  memcpy(key, md5(CAKEY), 32);
  memcpy(enTag, key, 16);
  memset(lenBuf, 0, 16);
  fstat(fileno(fp), &stat_buf);
  datalen = stat_buf.st_size;
  datap = (char*)malloc(datalen + 16);
  memset(datap, 0, sizeof(datap));
  fread(datap, datalen, 1, fp);
  fclose(fp);
  if(memcmp(datap, enTag, 16) == 0) {
    for(i=16; i<datalen; i++) {
      if(i<32)
        lenBuf[i-16] = datap[i];
      else
        datap[i-32] = datap[i];
    }
    screw_aes(0,datap,datalen,key,&datalen);
    datalen = atoi(lenBuf);
  }else if(STRICT_MODE){
    datalen = 0;
  }
  fp = tmpfile();

  if (datalen > 0) {
    fwrite(datap, datalen, 1, fp);
  } else {
    fwrite(STRICT_MODE_ERROR_MESSAGE, strlen(STRICT_MODE_ERROR_MESSAGE), 1, fp);
  }

  free(datap);

  rewind(fp);
  return fp;
}

ZEND_API zend_op_array *(*org_compile_file)(zend_file_handle *file_handle, int TSRMLS_DC);

ZEND_API zend_op_array *pm9screw_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
  FILE  *fp;
  char  fname[32];
  
  if (!file_handle || !file_handle->filename || strstr(file_handle->filename, ".phar") || strstr(file_handle->filename, "phar://")) {
   return org_compile_file(file_handle, type);
  }
  
  memset(fname, 0, sizeof fname);
  if (zend_is_executing(TSRMLS_C)) {
    if (get_active_function_name(TSRMLS_C)) {
      strncpy(fname, get_active_function_name(TSRMLS_C), sizeof fname - 2);
    }
  }
  if (fname[0]) {
    if ( strcasecmp(fname, "show_source") == 0
      || strcasecmp(fname, "highlight_file") == 0) {
      return NULL;
    }
  }

  fp = fopen(file_handle->filename, "rb");
  if (!fp) {
    fp = zend_fopen(file_handle->filename, &file_handle->opened_path);
    if (!fp) {
      return org_compile_file(file_handle, type);
    }
  }

  if (file_handle->type == ZEND_HANDLE_FP) fclose(file_handle->handle.fp);
  #ifdef ZEND_HANDLE_FD
    if (file_handle->type == ZEND_HANDLE_FD) close(file_handle->handle.fd);
  #endif
  file_handle->handle.fp = pm9screw_ext_fopen(fp);
  file_handle->type = ZEND_HANDLE_FP;

  return org_compile_file(file_handle, type);
}

zend_module_entry php_screw_plus_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  "php_screw_plus",
  NULL,
  PHP_MINIT(php_screw_plus),
  PHP_MSHUTDOWN(php_screw_plus),
  NULL,
  NULL,
  PHP_MINFO(php_screw_plus),
#if ZEND_MODULE_API_NO >= 20010901
  "1.5.0", /* Replace with version number for your extension */
#endif
  STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(php_screw_plus);

PHP_MINFO_FUNCTION(php_screw_plus)
{
  php_info_print_table_start();
  php_info_print_table_header(2, "php_screw_plus support", "enabled");
  php_info_print_table_header(2, "php_screw_plus version", "0.11");
  php_info_print_table_end();
}

PHP_MINIT_FUNCTION(php_screw_plus)
{
  CG(compiler_options) |= ZEND_COMPILE_EXTENDED_INFO;
  org_compile_file = zend_compile_file;
  zend_compile_file = pm9screw_compile_file;
  return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(php_screw_plus)
{
  CG(compiler_options) |= ZEND_COMPILE_EXTENDED_INFO;
  zend_compile_file = org_compile_file;
  return SUCCESS;
}
