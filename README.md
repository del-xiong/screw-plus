## screw plus + php7.2.6 + redis + memcached + swoole
```
@author QQ群友 今日在线
WEB暴露端口 9000
docker run -p 80:9000 -name web1 -v /data:/data/nginx/www screw-plus:php7.2.6-redis-memcached-swoole
docker exec -it web1 /bin/sh
```