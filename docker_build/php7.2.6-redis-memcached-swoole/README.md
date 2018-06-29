## screw plus + php7.2.6 + redis + memcached + swoole
```
@author QQ群友 今日在线
@size 800MB
```
```
// 拉取:
docker pull pastyouth/php-screw-plus:php7.2.6-redis-memcached-swoole
// 运行:
docker run -p 9000:9000 -name web1 -v /data:/data/nginx/www screw-plus:php7.2.6-redis-memcached-swoole
// 进入容器
docker exec -it web1 /bin/sh
```