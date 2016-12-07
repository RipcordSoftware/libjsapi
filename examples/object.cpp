#include <iostream>
#include "libjsapi.h"

int main() {
    rs::jsapi::Context cx;

    // create an object with a single field 'the_answer' with a getter callback which always returns 42
    rs::jsapi::Value obj(cx);
    rs::jsapi::Object::Create(cx, { "the_answer" },
        [](const char* name, rs::jsapi::Value& value) { value = 42; },
        nullptr,
        {},
        nullptr,
        obj);

    // create a function which returns the value of the field 'the_answer' on the passed object
    cx.Evaluate("var myfunc=function(o){return o.the_answer;};");

    rs::jsapi::FunctionArguments args(cx);
    args.Append(obj);

    // invoke the function and get the result
    rs::jsapi::Value result(cx);
    cx.Call("myfunc", args, result);

    // output the result to the console
    std::cout << result << std::endl;
}
