#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../php_screw_plus.h"
#include "../cauthcode.c"

void errMsg(char *str,char *str2) {
  printf("\033[40;31m%s%s\033[0m\n",str,str2);
}

void alertMsg(char *str,char *str2) {
  printf("\033[1;32m%s%s\033[0m\n",str,str2);
}

void encrypt(char *file);
void scanRoot(char *path);
int isPHP(char *filename);
main(int argc, char**argv)
{
    DIR *hP;
    FILE *fp;
    if (argc != 2) {
        errMsg("please input a valid path"," ");
        exit(1);
    }
    hP=opendir(argv[1]);
    if(hP == NULL) {
        fp = fopen(argv[1], "r");
        if(fp == NULL) {
            errMsg(argv[1]," is not a valid path ");
            exit(1);
        }else
            encrypt(argv[1]);
        
    }
    scanRoot(argv[1]);
}

void scanRoot(char *path)
{
    struct dirent* dir = NULL;
    DIR *hP;
    FILE *fp;
    int i;
    int l = 0;
    hP=opendir(path);
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
        if(dir->d_type == 4){
            if(curPath[l-1] != '/')
                strcat(curPath,"/");
            scanRoot(curPath);
        }else
        if(isPHP(dir->d_name) && dir->d_type == 8){
            encrypt(curPath);
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

void encrypt(char *file){
    FILE    *fp;
    struct  stat    stat_buf;
    char    *datap, *newdatap;
    int datalen, newdatalen;
    char    oldfilename[256];
    char *prepare;
    int i;


    fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "File not found(%s)", file);
        exit(0);
    }


    fstat(fileno(fp), &stat_buf);
    datalen = stat_buf.st_size;
    datap = (char*)malloc(maxBytes);
    fread(datap, datalen, 1, fp);
    fclose(fp);
    *(datap+datalen) = '\0';

    if (datalen>32 && strlen(cAuthCode(datap,false,CAKEY,0,32)) > 0) {
        errMsg(file ," Already Crypted");
        return ;
    }else if(datalen <1) {
        errMsg(file ," will not be crypted");
        return ;
    }

    newdatap = cAuthCode(datap,true,CAKEY,0,32);
    newdatalen = strlen(newdatap);

    fp = fopen(file, "w");
    if (fp == NULL) {
        alertMsg("Can not create crypt file(%s)", oldfilename);
        exit(0);
    }
    fwrite(newdatap, newdatalen, 1, fp);
    fclose(fp);
    alertMsg("Success Crypting - ", file);
    free(newdatap);
    free(datap);
}
