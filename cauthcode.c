#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>
#include "md5.h"
#include "b64.h"


int *cRange(int start, int end)
{
	int *str = (int *)calloc(end + 1, sizeof(int));
	int i;
	for(i = start; i <= end; i++)
	{
		str[i] = i;
	}
	return str;
}
char *c_substr(char *buffer, int start, int len)
{
  int buffer_len = strlen(buffer);
	if(buffer_len < 1)
		return "";
	char *tmp = (char *)calloc(buffer_len + 1, sizeof(char));
  int i = 0, j = 0;

  if(len == 95)
  {
    for(i = start; j < buffer_len; i++)
    {
      tmp[j] = buffer[i];
      j++;
    }
  }
  else
  {
    for(i = start; j < len; i++)
    {
      tmp[j] = buffer[i];
      j++;
    }
  }
	tmp[j] = '\0';
  return tmp;
}

static char *cAuthCode(char *str, bool operation, char *key, int expiry, int ckey_length)
{
	long tt;
	char *mKey, *keyA, *keyB, *keyC, t[11], cryptKey[80], tmp[maxBytes], keytmp[200];
	int keylen,  i, j, itmp = 0, a, lentypte = 0;
  int stringlen;
  int sl;
	int rndkey[257], decodelen;
	char *string = (char *)calloc(maxBytes, sizeof(char));

	bzero(t, sizeof(t));
	bzero(keytmp, sizeof(keytmp));
	bzero(rndkey, sizeof(rndkey));
	if(operation != true)
		operation = false;
	if(expiry <= 0)
		expiry = 0;

	tt = time(NULL);
	sprintf(t, "%ld", tt);
	mKey = md5(key);
	keyA = md5(c_substr(mKey, 0, 16));
	keyB = md5(c_substr(mKey, 16, 16));
	if(ckey_length > 0)
	{
		if(operation == false)
			keyC = c_substr(str, 0, ckey_length);
		else
			keyC = c_substr(md5(t), 32 - ckey_length, ckey_length);
	}
	else
	{
		keyC = "";
	}
	bzero(cryptKey, sizeof(cryptKey));
	sprintf(keytmp, "%s%s", keyA, keyC);
	sprintf(cryptKey, "%s%s", keyA, md5(keytmp));
	keylen = strlen(cryptKey);
	bzero(tmp, sizeof(tmp));
	sprintf(tmp, "%s%s", str, keyB);
	if(operation == true)
	{
		sprintf(string, "%010ld%s%s", (expiry > 0 ? expiry + tt : 0), c_substr(md5(tmp), 0, 16), str);
	   stringlen = strlen(string);
  }
	else
	{
		char *decode = (char *)calloc(strlen(str)-ckey_length, sizeof(char));
		decode = c_substr(str, ckey_length, strlen(str)-ckey_length);
		decodelen = strlen(decode);
		string = b64_decode(decode, strlen(decode),&sl);
		free(decode);
    		stringlen = sl;
	}
	if(stringlen == 0)
	{
		free(string);
		return "";
	}
	int *box = cRange(0, 255);
	for(i = 0; i <= 255; i++)
	{
		rndkey[i] = (int)(cryptKey[i % keylen]);
	}
	for(j = i = 0; i < 256; i++)
	{
		j = (j + box[i] + rndkey[i]) % 256;
		itmp = box[i];
		box[i] = box[j];
		box[j] = itmp;
	}

	unsigned int tmpresult[stringlen + 1];
	unsigned char result[stringlen + 1];
	bzero(tmpresult, sizeof(tmpresult));
	bzero(result, sizeof(result));
	for(a = j = i = 0; i < stringlen; i++)
	{
		a = (a + 1) % 256;
		j = (j + box[a]) % 256;
		itmp = box[a];
		box[a] = box[j];
		box[j] = itmp;
		tmpresult[i] = ((int)(string[i]) ^ (box[(box[a] + box[j]) % 256]));
	}
	for(i = 0; i < stringlen; i++)
	{
		result[i] = (char)tmpresult[i];
	}
	if(operation == true)
	{
		lentypte = i;
	}

	bzero(tmp, sizeof(tmp));	
	if(operation == false)
	{
		sprintf(tmp, "%s%s", c_substr(result, 26, 95), keyB);

		if((atoi(c_substr(result, 0, 10)) == 0 || atoi(c_substr(result, 0, 10)) - time(NULL) > 0))
		{
			if(strcmp(c_substr(result, 10, 16), c_substr(md5(tmp), 0, 16)) == 0)
			{
				free(string);
				return c_substr(result, 26, 95);
			}
			else
			{
				return "";
			}
		}
		else
		{
			free(string);
			return "";
		}
	}
	else
	{
		char *resul = (char *)calloc(maxBytes, sizeof(char));
		char *bencode;
		bencode = b64_encode(result, lentypte);

		sprintf(resul, "%s%s", keyC, bencode);
		free(string);
		return resul;
	}
}
