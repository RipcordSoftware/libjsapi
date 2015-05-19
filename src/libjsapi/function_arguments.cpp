/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Ripcord Software Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/

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
    JSAutoRequest ar(cx_);
    auto str = JS_NewStringCopyZ(cx_, value);
    auto strValue = JS::StringValue(str);
    return args_.append(strValue);    
}

bool rs::jsapi::FunctionArguments::Append(const std::string& value) {
    return Append(value.c_str());
}

bool rs::jsapi::FunctionArguments::Append(bool value) {
    return args_.append(JS::BooleanValue(value));
}

bool rs::jsapi::FunctionArguments::Append(const Value& value) {
    return args_.append(value);
}

bool rs::jsapi::FunctionArguments::Append(const JS::RootedObject& obj) {
    return args_.append(JS::ObjectOrNullValue(obj));
}

bool rs::jsapi::FunctionArguments::Append(const JS::RootedValue& value) {
    return args_.append(value);
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