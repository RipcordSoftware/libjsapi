#include <iostream>
#include <thread>

#include "libjsapi.h"

int main() {
    std::thread t1([]() {
        rs::jsapi::Runtime rt1;
        
        rs::jsapi::Script script1(rt1, "(function(){return 42;})();");
        script1.Compile();        

        rs::jsapi::Result result(rt1);
        script1.Execute(result);

        if (result.isNumber()) {
            auto val = result.toNumber();
            std::cout << val << std::endl;
        }
            
        rs::jsapi::Result result2(rt1);
        script1.Execute(result2);

        if (result.isNumber()) {
            auto val = result.toNumber();
            std::cout << val << std::endl;
        }
    });
    
    std::thread t2([]() {
        rs::jsapi::Runtime rt2;
        
        rs::jsapi::Script script2(rt2, "(function(){return 3.14159;})();");
        script2.Compile();

        rs::jsapi::Result result(rt2);
        script2.Execute(result);

        if (result.isNumber()) {
            auto val = result.toNumber();
            std::cout << val << std::endl;
        }
        
        auto context2b = rt2.NewContext();
        rs::jsapi::Script script2b(*context2b, "(function(){return 2.7182818;})();");
        script2b.Compile();

        rs::jsapi::Result result2b(*context2b);
        script2b.Execute(result2b);

        if (result2b.isNumber()) {
            auto val = result2b.toNumber();
            std::cout << val << std::endl;
        }
    });
    
    t1.join();
    t2.join();

    return 0;
}