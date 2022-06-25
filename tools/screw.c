#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <dirent.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../php_screw_plus.h"
#include "../aes.c"
#include "../aes_crypt.c"
#include "../md5.h"

void errMsg(char *str,char *str2) {
  printf("\033[40;31m%s%s\033[0m\n",str,str2);
}

void alertMsg(char *str,char *str2) {
  printf("\033[1;32m%s%s\033[0m\n",str,str2);
}

void screw_encrypt(char *file);
void screw_decrypt(char *file);
void scanRoot(char *path);
void screw_work(char *file);
int isPHP(char *filename);
int encode = 1;
uint8_t enTag[16];
uint8_t key[64];
void main(int argc, char**argv)
{
    DIR *hP;
    FILE *fp;
    char path[300];
    char suf[1];
    if (argc < 2) {
        errMsg("please input a valid path"," ");
        exit(1);
    }
    if(argc >2 && strncmp(argv[2],"-d",2) == 0)
      encode = 0;
    strcpy(path,argv[1]);
    if(!isPHP(argv[1]) && path[strlen(path)-1]!='/')
        strcat(path,"/");
    hP=opendir(path);
    if(hP == NULL) {
        fp = fopen(path, "r");
        if(fp == NULL) {
            errMsg(path," is not a valid path ");
            exit(1);
        }else
            screw_work(path);
        
    }
    scanRoot(path);
}

void scanRoot(char *path)
{
    struct dirent* dir = NULL;
    DIR *hP;
    FILE *fp;
    int i;
    int l = 0;
    hP=opendir(path);
    struct stat stat_buf;
    if(hP == NULL)
        return;
    while (NULL != (dir=readdir(hP)))
    {
        if(strncmp(dir->d_name,".",1) == 0 || strncmp(dir->d_name,"..",2) == 0)
            continue;
        l = strlen(path) + strlen(dir->d_name);
        char curPath[l+1];
        memset(curPath,0,sizeof(curPath));
        strcat(curPath,path);
        strcat(curPath,dir->d_name);
        stat(curPath, &stat_buf);
        // if it is a folder
        if (S_ISDIR(stat_buf.st_mode)) {
            if(curPath[l-1] != '/')
                strcat(curPath,"/");
            scanRoot(curPath);
        } else if (isPHP(dir->d_name)) {
          screw_work(curPath);
        }
    }
}

int isPHP(char *filename) {
    int i;
    char tmpS[5];
    memset(tmpS,0,sizeof(tmpS));
    for(i = 0;i<strlen(filename);i++)
        if(i >= strlen(filename)-4)
            tmpS[i - strlen(filename)+4] = filename[i];
    tmpS[4] = '\0';
    return strncmp(tmpS,".php",4)==0?1:0;
}

void screw_decrypt(char *file) {
  FILE  *fp;
  struct  stat  stat_buf;
  char  *datap;
  char  lenBuf[16];
  int i,datalen;
  uint8_t enTag[16];
  uint8_t key[64];
  fp = fopen(file, "rb+");
  if (fp == NULL) {
    errMsg("File not found(%s)", file);
    exit(0);
  }
  memset(key, 0, sizeof(key));
  memcpy(key, md5(CAKEY), 32);
  memcpy(enTag, key, 16);
  memset(lenBuf, 0, 16);
  fstat(fileno(fp), &stat_buf);
  datalen = stat_buf.st_size;
  datap = (char*)malloc(datalen);
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
    fp = fopen(file, "w+");
    fwrite(datap, datalen, 1, fp);
    free(datap);
    fclose(fp);
    alertMsg("Success Decrypting - ", file);
  }else {
    errMsg("Not a valid crypted file.","");
  }
  
}

void screw_work(char *file) {
  if(encode)
    screw_encrypt(file);
  else
    screw_decrypt(file);
}

void screw_encrypt(char *file) {
    FILE    *fp;
    struct  stat    stat_buf;
    char    *datap;
    int datalen;
    char    oldfilename[256];
    char *prepare;
    char lenBuf[16];
    int i;
    memset(lenBuf, 0, 16);
    memset(key, 0, sizeof(key));
    memcpy(key, md5(CAKEY), 32);
    memcpy(enTag, key, 16);

    fp = fopen(file, "rb");
    if (fp == NULL) {
        fprintf(stderr, "File not found(%s)", file);
        exit(0);
    }


    fstat(fileno(fp), &stat_buf);
    datalen = stat_buf.st_size;
    datap = (char*)malloc(datalen);
    memset(datap, 0, sizeof(datap));
    fread(datap, datalen, 1, fp);
    fclose(fp);
    sprintf(lenBuf,"%d",datalen);
    if (memcmp(datap, enTag, 16) == 0) {
        errMsg(file ," Already Crypted");
        return ;
    }else if(datalen <1) {
        errMsg(file ," will not be crypted");
        return ;
    }
    screw_aes(1,datap,datalen,key,&datalen);
    fp = fopen(file, "wb");
    if (fp == NULL) {
        errMsg("Can not create crypt file(%s)", oldfilename);
        exit(0);
    }
    fwrite(enTag, 16, 1, fp);
    fwrite(lenBuf, 16, 1, fp);
    fwrite(datap, datalen, 1, fp);
    fclose(fp);
    alertMsg("Success Crypting - ", file);
    free(datap);
}
