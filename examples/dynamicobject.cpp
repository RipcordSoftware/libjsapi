#include <iostream>
#include <vector>
#include <unordered_map>

#include "libjsapi.h"

int main() {
    std::unordered_map<std::string, std::string> data = {
        { "hello", "world" }, { "lorem", "ipsum" },
        { "foo", "bar" } };

    rs::jsapi::Runtime rt;

    // create a dynamic object which returns a string from the map
    rs::jsapi::Value obj(rt);
    rs::jsapi::DynamicObject::Create(rt,
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
    rt.Evaluate("var myfunc=function(o, n){return o[n];};");

    rs::jsapi::FunctionArguments args(rt);
    args.Append(obj);
    args.Append("hello");

    // invoke the function and get the value of the field 'hello'
    rs::jsapi::Value result(rt);
    rt.Call("myfunc", args, result);
    std::cout << result << std::endl;

    // invoke the function and get the value of the field 'lorem'
    args[1] = rs::jsapi::Value(rt, "lorem");
    rt.Call("myfunc", args, result);
    std::cout << result << std::endl;

    // invoke the function and get the value of the field 'foo'
    args[1] = rs::jsapi::Value(rt, "foo");
    rt.Call("myfunc", args, result);
    std::cout << result << std::endl;

    // invoke the function and get the value of the field 'xyz'
    args[1] = rs::jsapi::Value(rt, "xyz");
    rt.Call("myfunc", args, result);
    std::cout << result << std::endl;

    return 0;
}
