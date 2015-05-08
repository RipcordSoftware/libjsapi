#include <iostream>
#include "libjsapi.h"

int main() {
    rs::jsapi::Runtime rt;

    // create an object with a single field 'the_answer' with a getter callback which always returns 42
    rs::jsapi::Value obj(rt);
    rs::jsapi::Object::Create(rt, { "the_answer" },
        [](const char* name, rs::jsapi::Value& value) { value = 42; },
        nullptr,
        {},
        nullptr,
        obj);

    // create a function which returns the value of the field 'the_answer' on the passed object
    rt.Evaluate("var myfunc=function(o){return o.the_answer;};");

    rs::jsapi::FunctionArguments args(rt);
    args.Append(obj);

    // invoke the function and get the result
    rs::jsapi::Value result(rt);
    rt.Call("myfunc", args, result);

    // output the result to the console
    std::cout << result << std::endl;

    return 0;
}
