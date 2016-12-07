#include <iostream>
#include <vector>
#include <unordered_map>

#include "libjsapi.h"

int main() {
    std::unordered_map<std::string, std::string> data = {
        { "hello", "world" }, { "lorem", "ipsum" },
        { "foo", "bar" } };

    rs::jsapi::Context cx;

    // create a dynamic object which returns a string from the map
    rs::jsapi::Value obj(cx);
    rs::jsapi::DynamicObject::Create(cx,
        [&](const char* name, rs::jsapi::Value& value) {
            auto result = data.find(name);
            if (result != data.cend()) {
                value = result->second;
            } else {
                value.setUndefined();
            }
        },
        nullptr,
        nullptr,
        nullptr,
        obj);

    // create a function which returns the value of the field 'n'
    // on the passed object
    cx.Evaluate("var myfunc=function(o, n){return o[n];};");

    rs::jsapi::FunctionArguments args(cx);
    args.Append(obj);
    args.Append("hello");

    // invoke the function and get the value of the field 'hello'
    rs::jsapi::Value result(cx);
    cx.Call("myfunc", args, result);
    std::cout << result << std::endl;

    // invoke the function and get the value of the field 'lorem'
    args[1] = rs::jsapi::Value(cx, "lorem");
    cx.Call("myfunc", args, result);
    std::cout << result << std::endl;

    // invoke the function and get the value of the field 'foo'
    args[1] = rs::jsapi::Value(cx, "foo");
    cx.Call("myfunc", args, result);
    std::cout << result << std::endl;

    // invoke the function and get the value of the field 'xyz'
    args[1] = rs::jsapi::Value(cx, "xyz");
    cx.Call("myfunc", args, result);
    std::cout << result << std::endl;
}
