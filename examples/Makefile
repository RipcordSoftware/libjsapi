CXX:=g++

include ../src/make/coverage.mk
include ../src/make/ldlibs.mk
include ../src/make/libmozglue.mk

JSLIBS:=../src/libjsapi/dist/Debug/GNU-Linux-x86/libjsapi.a ../externals/installed/lib/libjs_static.ajs $(call LIBMOZGLUE, ../externals/installed/lib) $(LDLIBS)
CXXFLAGS:=--std=c++11 -I ../src/libjsapi -I ../externals/installed/include/mozjs-50/ $(JSLIBS) -lpthread `pkg-config --libs zlib` -ldl --coverage -g

LDFLAGS:=$(LDLIBS)

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
