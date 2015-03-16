#include <iostream>

#include "libjsapi.h"

int main() {

    rs::jsapi::Runtime rt;

    rs::jsapi::Script script(rt, "(function(){return 42;})();");
    script.Compile();
    
    if (rt.HasError()) {
        std::cout << rt.getError() << std::endl;
    } else {    
        rs::jsapi::Value result(rt);
        script.Execute(result);

        if (rt.HasError()) {
            std::cout << rt.getError() << std::endl;
        } else if (result().isNumber()) {
            auto val = result().toNumber();
            std::cout << val << std::endl;
        }
    }
    
    return 0;
}

