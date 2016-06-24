 /**
 * php screw plus
 **/
void screw_aes(int crypt,uint8_t *buf,int bufLen,uint8_t *key,int *rLen){
    uint8_t t,out;
    aes_context aes;
    int blocks = 0,i,rm = bufLen % 16;
    int end = 0,decSize=0;
    blocks = bufLen/16 + (rm?1:0);
    if(crypt)
        aes_setkey_enc( &aes, key, 256 );
    else
        aes_setkey_dec( &aes, key, 256 );
    for(i=0;i<blocks;i++) {
        if(crypt)
            aes_crypt_cbc(&aes, AES_ENCRYPT, 16, key, buf+i*16, buf+i*16);
        else
            aes_crypt_cbc(&aes, AES_DECRYPT, 16, key, buf+i*16, buf+i*16);
    }
    *rLen = blocks * 16;
}