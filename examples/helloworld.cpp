#include <iostream>
#include "libjsapi.h"

int main() {
    rs::jsapi::Runtime rt;

    // define a global function called echo which
    // prints the first argument on stdout
    rs::jsapi::Global::DefineFunction(rt, "echo",
        [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
            if (args.size() > 0) {
                std::cout << args[0] << std::endl;
            }
        });

    // invoke echo passing a string
    rs::jsapi::FunctionArguments args(rt);
    args.Append("Hello world!!");
    rt.Call("echo", args);

    // call the function from JavaScript
    rt.Evaluate("echo('lorem ipsum...');");

    return 0;
}
