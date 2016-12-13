COVERAGE_FLAGS:=$(shell cd /tmp; echo 'int main(){}' | $(CXX) -x c++ -o /dev/null --coverage - > /dev/null 2>&1 && echo '--coverage')
