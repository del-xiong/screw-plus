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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "php_screw_plus.h"
#include "cauthcode.c"

PHP_MINIT_FUNCTION(php_screw_plus);
PHP_MSHUTDOWN_FUNCTION(php_screw_plus);
PHP_MINFO_FUNCTION(php_screw_plus);

FILE *pm9screw_ext_fopen(FILE *fp)
{
	struct	stat	stat_buf;
	char	*datap, *newdatap;
	int	datalen, newdatalen=0;
	int	i;

	fstat(fileno(fp), &stat_buf);
	datalen = stat_buf.st_size;
	datap = (char*)malloc(maxBytes);
	fread(datap, datalen, 1, fp);
	fclose(fp);
	if(datalen > 32) {
		newdatap = cAuthCode(datap,false,CAKEY,0,32);
		newdatalen = strlen(newdatap);
	}
	fp = tmpfile();

	if(newdatalen>0) {
		fwrite(newdatap, newdatalen, 1, fp);
		free(newdatap);
	}else
		fwrite(datap, datalen, 1, fp);
	free(datap);

	rewind(fp);
	return fp;
}

ZEND_API zend_op_array *(*org_compile_file)(zend_file_handle *file_handle, int type TSRMLS_DC);

ZEND_API zend_op_array *pm9screw_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
	FILE	*fp;
	char	fname[32];

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

	fp = fopen(file_handle->filename, "r");
	if (!fp) {
		return org_compile_file(file_handle, type);
	}

	if (file_handle->type == ZEND_HANDLE_FP) fclose(file_handle->handle.fp);
	if (file_handle->type == ZEND_HANDLE_FD) close(file_handle->handle.fd);
	file_handle->handle.fp = pm9screw_ext_fopen(fp);
	file_handle->type = ZEND_HANDLE_FP;
	file_handle->opened_path = expand_filepath(file_handle->filename, NULL TSRMLS_CC);

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
