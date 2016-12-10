CXX:=g++
JSLIBS:=../src/libjsapi/dist/Debug/GNU-Linux-x86/libjsapi.a ../externals/installed/lib/libjs_static.ajs -Wl,--whole-archive ../externals/installed/lib/libmozglue.a -Wl,--no-whole-archive
CXXFLAGS:=--std=c++11 -I ../src/libjsapi -I ../externals/installed/include/mozjs-50/ $(JSLIBS) -lpthread `pkg-config --libs zlib` -ldl --coverage -g

LDLIBS:=$(shell echo 'int main(){}' | $(CXX) -x c++ -o /dev/null -lrt - > /dev/null 2>&1 && echo '-lrt')
LDFLAGS+=$(LDLIBS)

all: helloworld square object dynamicobject dynamicarray

.PHONY: clean

helloworld: helloworld.cpp
	$(CXX) $? $(CXXFLAGS) $(LDFLAGS) -o $@
	
square: square.cpp
	$(CXX) $? $(CXXFLAGS) $(LDFLAGS) -o $@
	
object: object.cpp
	$(CXX) $? $(CXXFLAGS) $(LDFLAGS) -o $@

dynamicobject: dynamicobject.cpp
	$(CXX) $? $(CXXFLAGS) $(LDFLAGS) -o $@

dynamicarray: dynamicarray.cpp
	$(CXX) $? $(CXXFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f helloworld square object dynamicobject dynamicarray
