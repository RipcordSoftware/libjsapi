/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Ripcord Software Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/

#include <iostream>
#include <thread>
#include <vector>

#include "libjsapi.h"

int main() {
    std::vector<std::thread> threads;

    threads.emplace_back([] {
        rs::jsapi::Runtime rt1;
        
        rs::jsapi::Script script1(rt1, "(function(){return 42;})();");
        script1.Compile();        

        rs::jsapi::Value result(rt1);
        script1.Execute(result);

        if (result.isNumber()) {
            auto val = result.toNumber();
            std::cout << val << std::endl;
        }
            
        rs::jsapi::Value result2(rt1);
        script1.Execute(result2);

        if (result.isNumber()) {
            auto val = result.toNumber();
            std::cout << val << std::endl;
        }
    });
    
    threads.emplace_back([] {
        rs::jsapi::Runtime rt2;
        
        rs::jsapi::Script script2(rt2, "(function(){return 3.14159;})();");
        script2.Compile();

        rs::jsapi::Value result(rt2);
        script2.Execute(result);

        if (result.isNumber()) {
            auto val = result.toNumber();
            std::cout << val << std::endl;
        }
        
        auto context2b = rt2.NewContext();
        rs::jsapi::Script script2b(*context2b, "(function(){return 2.7182818;})();");
        script2b.Compile();

        rs::jsapi::Value result2b(*context2b);
        script2b.Execute(result2b);

        if (result2b.isNumber()) {
            auto val = result2b.toNumber();
            std::cout << val << std::endl;
        }
    });
    
    for (auto& thread : threads) {
        thread.join();
    }
}