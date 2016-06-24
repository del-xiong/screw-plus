#php screw plus
### 简述

2016/06/24更新:改进了加密算法 使用高强度的AES256 CBC模式进行加密 

基于php screw  
主要做了以下改进：  
 **增加了批量加密 更加方便**  
 **原生加密算法太弱易被破解,升级了加密算法，更加安全可靠**  
加密的优点：  
 **可防止源码泄露，本程序是php扩展加密运行，只能在vps上运行，由于需要扩展进行动态解密，加密后被别人全部下载也无法被破解**  
 **商业源码可加密核心功能，只开放接口给客户**  
 **部分公司需要团队开发的，可防止源码泄露**  
 **其他。。。我编不出来了**  
### 效果演示  
原生代码:  
```
<?php
if(!defined('IN_COMSENZ')) {
  exit('Access Denied');
}

define('SOFT_NAME', 'Discuz!');

define('INSTALL_LANG', 'SC_UTF8');

define('CONFIG', './config/config_global.php');
define('CONFIG_UC', './config/config_ucenter.php');
```
加密后：  
```
256898d5d2b58de39c83b328c50f3490MPd0CerqcfOp2udEtjBUzttFgrvEezREJS6Hq0BAmXF52HrtnrE8susTcLQIS+KoHARxdLY7tYAbBpTwNw8kdTWFT8bl7EO4b3p8kH1IL2Yz0TwfyWmcZpWCcuYPccUZDw7JDJzT2CqyH6ZTTksRuQ6H6lgpUB9LWL1+Djb1XmPtgPhGYIAgRMnhhyljc3crJ6vepk1x4w9aoKEbMNF7b4T2kVLqyOAEng4Dn/2liVBWRQvB1fztHQdiJcr0TAnTOCQKYaSA3KVnXhY/pxnk8/fQm+Dv+i1kJcpTT3JPTFk7yRyJ3T/6VX5jgl6PRs7cLewm/2+stylvxmcreCHP+7nLxkoBqbBrangq+C+9E8+4dEMYsc7xpKOFUMvuj4VhksdCO5nLd+FmkVJtwAYBK8Ac4vfKlobCgfdikFlkaTCFLVLt9ZTObcWuN5nPHyq/XhXF/N/gxh5vYL1FEBTeobZkzovDqt2fZChjO44875VapxKGf0qE+96M9uxj9XiJ0P09cAcuJEGfPCyNy81Mb2oMjJl+lqLFWCevw3i8U3BpBTeOMwYkyzbJlxznQVY9Q5JjaWQO76XBjsLtp6cHNMz9lnCP7viiFrirIP13uuoESRVe5FiXJvmuuEdM+iRdmKw9JIQsjzCH447NRlilC/Ls9ECGYcrImH1KG/tfrAXXmfCmRuqS5
```
### 使用方法：  
1. 下载本程序并解压到某个目录
2. 在screw plus目录中执行php bin中的phpize自动生成扩展所需文件（如果你的php里没有可以去官网下载）
3. 执行./configure --with-php=config=[php config path] 进行配置，[php config path]是你的php-config的绝对路径
4. 修改php_screw_plus.h中的CAKEY，改为一个你认为安全的字符串
5. 执行make生成扩展 modules/php_screw_plus.so
6. 把扩展路径加入php.ini中 重启php
7. 进入tools文件夹 执行make
8. 执行./screw [目录或文件] ，后面带上你要加密的目录或文件即可自动开始加密

现在可以访问php文件，如果前面没有发生错误，应该可以正常执行了。
