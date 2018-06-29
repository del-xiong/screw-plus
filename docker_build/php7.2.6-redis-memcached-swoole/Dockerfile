FROM centos

# 安装工具
RUN cd / && yum install -y epel-release && \
yum install -y wget vi gcc gcc-c++ autoconf libjpeg libcurl libcurl-devel libjpeg-devel libpng libpng-devel freetype freetype-devel libxml2 libxml2-devel \
zlib zlib-devel glibc glibc-devel glib2 glib2-devel bzip2 bzip2-devel ncurses ncurses-devel curl curl-devel e2fsprogs e2fsprogs-devel krb5 krb5-devel \
libidn libidn-devel openssl openssl-devel openldap openldap-devel nss_ldap openldap-clients openldap-servers libxslt libxslt-devel libevent-devel \
iconv libmcrypt libmcrypt-devel libicu-devel re2c bison bzip2 bzip2-devel unzip; yum clean all && \
ldconfig

# 安装libzip的安装工具cmake https://cmake.org/files/
RUN cd / && wget https://cmake.org/files/v3.12/cmake-3.12.0-rc1.tar.gz && \
tar -zxvf cmake-3.12.0-rc1.tar.gz && \
cd /cmake-3.12.0-rc1 && \
./configure && make && make install && \
rm -f /cmake-3.12.0-rc1.tar.gz && \
rm -rf /cmake-3.12.0-rc1

# 安装libzip库 https://libzip.org/download/
RUN cd / && wget https://libzip.org/download/libzip-1.5.1.tar.gz && \
tar -xf libzip-1.5.1.tar.gz && \
cd /libzip* && \
mkdir build && cd build && cmake .. && make && make install && \
rm -f /libzip-1.5.1.tar.gz && \
rm -rf /libzip-1.5.1

# 安装ICU库(为了安装intl扩展 owncloud用到)
RUN cd / && wget http://download.icu-project.org/files/icu4c/62rc/icu4c-62rc-src.tgz && \
tar xf icu4c-62rc-src.tgz && \
cd icu/source && \
mkdir /usr/local/icu && \
./configure --prefix=/usr/local/icu && \
make && make install && \
rm -f /icu4c-62rc-src.tgz && \
rm -rf /icu

# 下载并解压源码包
RUN cd / && wget http://jp2.php.net/distributions/php-7.2.6.tar.gz && \
tar -zxvf php-7.2.6.tar.gz
# 编译安装
RUN cd /php-7.2.6 && ./configure --prefix=/usr/local/php \
 --with-curl \
 --with-freetype-dir \
 --with-jpeg-dir \
 --with-gd \
 --with-gettext \
 --with-iconv-dir \
 --with-kerberos \
 --with-libdir=lib64 \
 --with-libxml-dir \
 --with-mysqli \
 --with-openssl \
 --with-pcre-regex \
 --with-pdo-mysql \
 --with-pdo-sqlite \
 --with-pear \
 --with-png-dir \
 --with-xmlrpc \
 --with-xsl \
 --with-zlib \
 --with-mhash \
 --with-libzip \
 --enable-fpm \
 --enable-bcmath \
 --enable-libxml \
 --enable-inline-optimization \
 --enable-mbregex \
 --enable-mbstring \
 --enable-opcache \
 --enable-pcntl \
 --enable-shmop \
 --enable-soap \
 --enable-sockets \
 --enable-sysvsem \
 --enable-xml \
 --enable-zip \
 --enable-simplexml \
 --enable-intl && \
make && make install && \
cp php.ini-production /usr/local/php/lib/php.ini && \
cp sapi/fpm/init.d.php-fpm /usr/local/php/etc/php-fpm && \
sed -i 's/ --daemonize//g' /usr/local/php/etc/php-fpm && \
chmod +x /usr/local/php/etc/php-fpm && \
cp /usr/local/php/etc/php-fpm.conf.default /usr/local/php/etc/php-fpm.conf && \
cp /usr/local/php/etc/php-fpm.d/www.conf.default /usr/local/php/etc/php-fpm.d/www.conf && \
sed -i 's/^;daemonize = yes/daemonize = no/g' /usr/local/php/etc/php-fpm.conf && \
sed -i 's/^listen = 127\.0\.0\.1\:9000/listen = 9000/g' /usr/local/php/etc/php-fpm.d/www.conf && \
rm -rf /php-7.2.6 && \
rm -f /php-7.2.6.tar.gz


#下载mcrypt扩展
RUN cd / && wget http://pecl.php.net/get/mcrypt-1.0.1.tgz && \
tar -zxvf mcrypt-1.0.1.tgz && \
cd mcrypt-1.0.1 && \
/usr/local/php/bin/phpize && \
./configure --with-php-config=/usr/local/php/bin/php-config && \
make && make install && \
echo "extension_dir=/usr/local/php/lib/php/extensions/no-debug-non-zts-20170718/">>/usr/local/php/lib/php.ini && \
echo "extension=mcrypt.so">>/usr/local/php/lib/php.ini && \
rm -rf /mcrypt-1.0.1 && \
rm -f /mcrypt-1.0.1.tgz

# 安装memcached 依赖库igbinary http://pecl.php.net/package/igbinary
RUN cd / && wget http://pecl.php.net/get/igbinary-2.0.6.tgz && \
tar -zxvf igbinary-2.0.6.tgz && \
cd igbinary-2.0.6 && \
/usr/local/php/bin/phpize && \
./configure --with-php-config=/usr/local/php/bin/php-config && \
make && make install && \
echo "extension=igbinary.so">>/usr/local/php/lib/php.ini && \
rm -f /igbinary-2.0.6.tgz && \
rm -rf /igbinary-2.0.6

# 安装memcached 依赖库msgpack http://pecl.php.net/package/msgpack
RUN cd / && wget http://pecl.php.net/get/msgpack-2.0.2.tgz && \
tar -zxvf msgpack-2.0.2.tgz && \
cd msgpack-2.0.2 && \
/usr/local/php/bin/phpize && \
./configure --with-php-config=/usr/local/php/bin/php-config && \
make && make install && \
echo "extension=msgpack.so">>/usr/local/php/lib/php.ini && \
rm -f /msgpack-2.0.2.tgz && \
rm -rf /msgpack-2.0.2

# 安装redis依赖库hiredis https://github.com/redis/hiredis/releases
RUN cd / && wget https://github.com/redis/hiredis/archive/v0.13.3.tar.gz && \
tar -zxvf v0.13.3.tar.gz && \
cd hiredis-0.13.3/ && \
make && make install && \
ldconfig && \
rm -f /v0.13.3.tar.gz && \
rm -rf /hiredis-0.13.3

# 安装redis https://github.com/phpredis/phpredis/releases
RUN cd / && wget https://github.com/phpredis/phpredis/archive/4.1.0RC1.tar.gz && \
tar -zxvf 4.1.0RC1.tar.gz && \
cd phpredis-4.1.0RC1/ && \
/usr/local/php/bin/phpize && \
./configure --with-php-config=/usr/local/php/bin/php-config && \
make && \
make install && \
echo "extension=redis.so">>/usr/local/php/lib/php.ini && \
rm -f /4.1.0RC1.tar.gz && \
rm -rf /phpredis-4.1.0RC1

# 安装 http2依赖库 nghttp2   https://github.com/nghttp2/nghttp2/releases/
RUN cd / && wget https://github.com/nghttp2/nghttp2/releases/download/v1.32.0/nghttp2-1.32.0.tar.gz && \
tar -zxvf nghttp2-1.32.0.tar.gz && \
cd nghttp2-1.32.0/ && \
./configure && \
make && make install && \
rm -f /nghttp2-1.32.0.tar.gz && \
rm -rf /nghttp2-1.32.0

# 安装swoole https://github.com/swoole/swoole-src/releases
RUN cd / && wget https://github.com/swoole/swoole-src/archive/v4.0.0.tar.gz && \
tar -zxvf v4.0.0.tar.gz && \
cd swoole-src-4.0.0/ && \
/usr/local/php/bin/phpize && \
./configure --enable-sockets --enable-openssl --enable-http2 --enable-async-redis --enable-timewheel --enable-mysqlnd --enable-coroutine --with-php-config=/usr/local/php/bin/php-config && \
make clean && make && make install && \
echo "extension=swoole.so">>/usr/local/php/lib/php.ini && \
echo "/usr/local/lib">>/etc/ld.so.conf && \
ldconfig && \
rm -f /v4.0.0.tar.gz && \
rm -rf /swoole-src-4.0.0

# 安装memcached 依赖库 libmemcached  https://launchpad.net/libmemcached/
RUN cd / && wget https://launchpad.net/libmemcached/1.0/1.0.18/+download/libmemcached-1.0.18.tar.gz && \
tar -zxvf libmemcached-1.0.18.tar.gz && \
cd libmemcached-1.0.18 && \
./configure --prefix=/usr/local/libmemcached && \
make && make install && \
rm -f /libmemcached-1.0.18.tar.gz && \
rm -rf /libmemcached-1.0.18

# 安装http://pecl.php.net/package/memcached
RUN cd / && wget http://pecl.php.net/get/memcached-3.0.4.tgz && \
tar -zxvf memcached-3.0.4.tgz && \
cd memcached-3.0.4 && \
/usr/local/php/bin/phpize && \
./configure --with-php-config=/usr/local/php/bin/php-config  --with-libmemcached-dir=/usr/local/libmemcached --enable-memcached-igbinary \
--enable-memcached-json --enable-memcached-msgpack  && \
make && make install && \
echo "extension=memcached.so">>/usr/local/php/lib/php.ini && \
rm -f /memcached-3.0.4.tgz && \
rm -rf /memcached-3.0.4

# 安装 php screw plus  https://codeload.github.com/del-xiong/screw-plus/zip/master
RUN cd / && wget https://codeload.github.com/del-xiong/screw-plus/zip/master && \
unzip master && \
cd /screw-plus-master && \
/usr/local/php/bin/phpize && \
./configure --with-php-config=/usr/local/php/bin/php-config && \
make && \
cp /screw-plus-master/modules/php_screw_plus.so /usr/local/php/lib/php/extensions/no-debug-non-zts-20170718/ && \
echo "extension=php_screw_plus.so">>/usr/local/php/lib/php.ini && \
cd /screw-plus-master/tools && \
make && \
rm -f /master
#rm -rf /screw-plus-master

# 创建好挂载目录
RUN mkdir -p /data/php/log && mkdir -p /data/nginx/www

# 设置时区
RUN cp -f /usr/share/zoneinfo/Asia/Shanghai /etc/localtime && echo "Asia/Shanghai" > /etc/timezone

# 启动php服务
RUN chmod +x /usr/local/php/etc/php-fpm && /usr/local/php/etc/php-fpm start &

# 设置环境变量
ENV PATH=$PATH:/usr/local/php/etc:/usr/local/php/bin

#设置生成容器时需要执行的脚本
#ENTRYPOINT ["/usr/local/php/sbin/php-fpm"]
CMD [ "php-fpm", "start" ]

#开放9000端口
EXPOSE 9000
