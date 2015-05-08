#include <iostream>
#include <vector>

#include "libjsapi.h"

int main() {
    std::vector<int> data = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144 };

    rs::jsapi::Runtime rt;

    // create a dynamic array which exposes the `data` vector to JS
    rs::jsapi::Value array(rt);
    rs::jsapi::DynamicArray::Create(rt,
        [&](int index, rs::jsapi::Value& value) { value = data[index]; },
        nullptr,
        [&]() { return data.size(); },
        nullptr,
        array);

    // create a function which returns the value of the item 'n'
    // on the passed array
    rt.Evaluate("var myfunc=function(arr, n){ return arr[n]; }");

    // create an arguments instance so we can call the JS function
    rs::jsapi::FunctionArguments args(rt);
    args.Append(array);
    args.Append(0);

    // invoke the function and get the value of the index i
    for (int i = 0; i < data.size(); ++i) {
        args[1].setInt32(i);

        rs::jsapi::Value result(rt);
        rt.Call("myfunc", args, result);

        std::cout << result << std::endl;
    }

    return 0;
}
