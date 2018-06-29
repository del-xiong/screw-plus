## alpine+php7.1+screw plus
```
@author QQ群友 chunhui.hu
@size 88.6MB
拉取: docker pull pastyouth/php-screw-plus:php7.1_alpine
运行方法: docker run -p ... -v ...
构建完成后删除了screw plus编译环境, 建议外部挂载程序
若要内部重新编译screw plus, 可注释Dockerfile最后一行 apk del..., 构建镜像成功后可在容器内部编译, 容量会上升至300MB左右
```