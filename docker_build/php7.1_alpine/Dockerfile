FROM alpine:3.7

RUN echo "https://mirrors.aliyun.com/alpine/v3.7/main/" > /etc/apk/repositories \
&& echo "https://mirrors.aliyun.com/alpine/v3.7/community/" >> /etc/apk/repositories \
&& apk add --update --no-cache rsyslog runit dcron curl wget bash rsync nodejs tzdata \
&& apk add php7 php7-fpm php7-gd php7-redis \
php7-opcache php7-pdo php7-pdo_mysql php7-mbstring php7-json \
php7-mcrypt php7-curl php7-ctype php7-zip php7-xml php7-simplexml \
php7-xmlreader php7-xmlwriter php7-phar php7-fileinfo

RUN apk add --virtual .build-deps build-base php7-dev zlib-dev && \
cd / && \
wget https://codeload.github.com/del-xiong/screw-plus/zip/master && \
unzip master && \
cd /screw-plus-master && \
phpize && \
./configure --with-php-config=php-config && \
make && make install && \
cd tools && make && \
apk del .build-deps