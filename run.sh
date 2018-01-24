#!/bin/sh
echo '更新文件...'
#git fetch --all
#git reset --hard origin/master

echo '编译C++文件...'

echo '设置LIB目录...'
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib

echo '编译JAVA文件...'
mvn package
echo '运行...'
cp -Rf lib target/
java -jar target/camera-1.jar
