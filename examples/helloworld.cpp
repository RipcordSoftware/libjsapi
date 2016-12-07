#include <iostream>
#include "libjsapi.h"

int main() {
    rs::jsapi::Context cx;

    // define a global function called echo which
    // prints the first argument on stdout
    rs::jsapi::Global::DefineFunction(cx, "echo",
        [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
            if (args.size() > 0) {
                std::cout << args[0] << std::endl;
            }
        });

    // invoke echo passing a string
    rs::jsapi::FunctionArguments args(cx);
    args.Append("Hello world!!");
    cx.Call("echo", args);

    // call the function from JavaScript
    cx.Evaluate("echo('lorem ipsum...');");
}
