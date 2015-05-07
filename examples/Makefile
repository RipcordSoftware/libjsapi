CXX=g++
CXXFLAGS=--std=c++11 -I ../src/libjsapi -I ../externals/installed/include/mozjs-31/ ../src/libjsapi/dist/Debug/GNU-Linux-x86/libjsapi.a ../externals/installed/lib/libmozjs-31.a -l pthread `pkg-config --libs zlib` -l dl --coverage -g

all: helloworld square object dynamicobject dynamicarray

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
	rm helloworld square object dynamicobject dynamicarray
