#!/bin/sh
echo '更新文件...'
git pull
echo '编译文件...'
rm -Rf target
mvn clean package
echo '运行...'
cp -Rf lib target/
java -jar target/camera-1.jar 