#!/bin/bash

for f in *.cpp; do
    g++ -o `basename -s .cpp ${f}` --std=c++11 ${f} -I ../src/libjsapi -I ../externals/installed/include/mozjs-31/ ../src/libjsapi/dist/Debug/GNU-Linux-x86/libjsapi.a ../externals/installed/lib/libmozjs-31.a -l pthread `pkg-config --libs zlib` -l dl --coverage
done