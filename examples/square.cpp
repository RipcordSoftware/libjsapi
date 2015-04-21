#include <iostream>
#include "libjsapi.h"

int main() {
    rs::jsapi::Runtime rt;

    // define a function which squares the first argument and returns the result
    rs::jsapi::Global::DefineFunction(rt, "square",
        [](JSContext* cx, unsigned argc, JS::Value* vp) {
            auto args = JS::CallArgsFromVp(argc, vp);
            rs::jsapi::Value x(cx, args.get(0));
            args.rval().setNumber(x.toNumber() * x.toNumber());
            return true;
        });

    // call the function via JSAPI
    rs::jsapi::FunctionArguments args(rt);
    args.Append(5);
    rs::jsapi::Value result(rt);
    rt.Call("square", args, result);
    std::cout << result << std::endl;

    // call the function from JavaScript and return the result
    rt.Evaluate("(function(){return square(9);})();", result);
    std::cout << result << std::endl;

    return 0;
}
