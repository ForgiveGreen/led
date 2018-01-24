#!/bin/sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
rm -Rf test
rm -Rf libHvDeviceJava.so
echo '更新代码'
#git pull
echo '编译运行文件'
g++ main.cpp CameraOpt.cpp HttpRequest.cpp -w -o test -L. -lHvDevice
echo '编译动态库'
g++ main.cpp CameraOpt.cpp HttpRequest.cpp -w -fPIC -shared -o libHvDeviceJava.so  -L. -lHvDevice
echo '运行'
./test