LDLIBS:=$(shell echo 'int main(){}' | $(CXX) -x c++ -o /dev/null -ldl - > /dev/null 2>&1 && echo '-ldl')
LDLIBS+=$(shell echo 'int main(){}' | $(CXX) -x c++ -o /dev/null -lrt - > /dev/null 2>&1 && echo '-lrt')
