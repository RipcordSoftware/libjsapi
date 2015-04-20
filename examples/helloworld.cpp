#include <iostream>
#include "libjsapi.h"

int main() {
    rs::jsapi::Runtime rt;

    // define a global function called echo which
    // prints the first argument on stdout
    rs::jsapi::Global::DefineFunction(rt, "echo",
        [](JSContext* cx, unsigned argc, JS::Value* vp) {
            auto args = JS::CallArgsFromVp(argc, vp);
            if (argc > 0) {
                rs::jsapi::Value str(cx, args.get(0));
                std::cout << str.ToString() << std::endl;
            }
            return true;
        });

    // invoke echo passing a string
    rs::jsapi::FunctionArguments args(rt);
    args.Append("Hello world!!");
    rt.Call("echo", args);

    // call the function from JavaScript
    rt.Evaluate("echo('lorem ipsum...');");

    return 0;
}
