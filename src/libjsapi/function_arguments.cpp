#include "function_arguments.h"
#include "context.h"
#include "value.h"
#include "object.h"

rs::jsapi::FunctionArguments::FunctionArguments(Context& cx) : cx_(cx), args_(cx_) {
    
}

bool rs::jsapi::FunctionArguments::Append(signed value) {
    return args_.append(JS::Int32Value(value));
}

bool rs::jsapi::FunctionArguments::Append(unsigned value) {
    return args_.append(JS::DoubleValue(value));
}

bool rs::jsapi::FunctionArguments::Append(double value) {
    return args_.append(JS::DoubleValue(value));
}

bool rs::jsapi::FunctionArguments::Append(const char* value) {
    auto str = JS_NewStringCopyZ(cx_, value);
    auto strValue = JS::StringValue(str);
    return args_.append(strValue);    
}

bool rs::jsapi::FunctionArguments::Append(bool value) {
    return args_.append(JS::BooleanValue(value));
}

bool rs::jsapi::FunctionArguments::Append(Value& value) {
    return args_.append(value);
}

bool rs::jsapi::FunctionArguments::Append(const JS::RootedObject& obj) {
    return args_.append(JS::ObjectOrNullValue(obj));
}

void rs::jsapi::FunctionArguments::Clear() {
    args_.clear();
}

bool rs::jsapi::FunctionArguments::Empty() {
    return args_.empty();
}

int rs::jsapi::FunctionArguments::getLength() {
    return args_.length();
}

JS::Value& rs::jsapi::FunctionArguments::operator [](int index) {
    if (index < 0 || index >= args_.length()) {
        throw FunctionArgumentsIndexException();
    }
    
    return args_[index]; 
}