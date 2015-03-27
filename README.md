[![Build Status](https://travis-ci.org/RipcordSoftware/libjsapi.svg?branch=master)](https://travis-ci.org/RipcordSoftware/libjsapi)
[![Coverage Status](https://coveralls.io/repos/RipcordSoftware/libjsapi/badge.svg)](https://coveralls.io/r/RipcordSoftware/libjsapi)

# libjsapi
A SpiderMonkey 31 (Mozilla) JSAPI wrapper for C++ 11.

The JSAPI interface to the SpiderMonkey JavaScript VM is difficult to integrate into 
applications without an intermediate abstraction layer managing object creation and 
type mapping.

Example:
```c++
#include <iostream>
#include "libjsapi.h"

void main() {
    rs::jsapi::Runtime rt;
    
    rs::jsapi::Script script(rt, "(function(){return 42;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt);
    script.Execute(result);
    
    auto val = result().toNumber();
    std::cout << val << std::endl;
}
```

# Building
Requires:
* GCC 4.8.1+
* GNU Make
* lcov and Ruby (for test coverage)
* valgrind (for memory analysis)

To build type:
```bash
$ make
```

To build tests with coverage:
```bash
$ make test
```

To create a coverage report:
```bash
$ ./coverage.sh
```
