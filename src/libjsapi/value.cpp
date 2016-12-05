/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Ripcord Software Ltd
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

#include "value.h"
#include "context.h"
#include "function_arguments.h"

#include "js/Conversions.h"

#include <vector>

rs::jsapi::Value::Value(JSContext* cx) : cx_(cx), isObject_(false), value_(cx), object_(cx) {

}

rs::jsapi::Value::Value(const Value& rhs) : Value(rhs.cx_) {
    isObject_ = rhs.isObject_;
    if (rhs.isObject_) {
        object_ = rhs.object_;
    } else {
        value_ = rhs.value_;
    }
}

rs::jsapi::Value::~Value() {
}

rs::jsapi::Value::Value(JSContext* cx, const char* str) : Value(cx) {
    set(str);
}

rs::jsapi::Value::Value(JSContext* cx, const std::string& str) : Value(cx) {
    set(str);
}

rs::jsapi::Value::Value(JSContext* cx, int value): Value(cx) {
    set(value);
}

rs::jsapi::Value::Value(JSContext* cx, bool value) : Value(cx) {
    set(value);
}

rs::jsapi::Value::Value(JSContext* cx, double value) : Value(cx) {
    set(value);
}

rs::jsapi::Value::Value(JSContext* cx, const JS::HandleValue& value) : Value(cx) {
    set(value);
}

rs::jsapi::Value::Value(JSContext* cx, const JS::RootedObject& value) : cx_(cx), isObject_(true), value_(cx), object_(cx, value) {

}

rs::jsapi::Value::Value(JSContext* cx, JSObject* value) : cx_(cx), isObject_(true), value_(cx), object_(cx, value) {

}

rs::jsapi::Value::Value(JSContext* cx, JSString* value) : cx_(cx), isObject_(false), value_(cx), object_(cx, nullptr) {
    value_.setString(value);
}

rs::jsapi::Value::Value(JSContext* cx, const JS::HandleObject& value) : cx_(cx), isObject_(true), value_(cx), object_(cx, value) {

}

void rs::jsapi::Value::set(const Value& value) {
    *this = value;
}

void rs::jsapi::Value::set(const JS::HandleValue& value) {
    InitValueRef();
    value_ = value;
}

void rs::jsapi::Value::set(const JS::RootedObject& value) {
    InitObjectRef();
    object_ = value;
}

void rs::jsapi::Value::set(const JS::HandleObject& value) {
    InitObjectRef();
    object_ = value;
}

void rs::jsapi::Value::set(JSObject* value) {
    InitObjectRef();
    object_ = value;
}

void rs::jsapi::Value::set(JSString* value) {
    InitValueRef();
    value_.setString(value);
}

void rs::jsapi::Value::set(const char* str) {
    InitValueRef();
    value_.setString(JS_NewStringCopyZ(cx_, str));
}

void rs::jsapi::Value::set(const std::string& str) {
    InitValueRef();
    value_.setString(JS_NewStringCopyN(cx_, str.c_str(), str.length()));
}

void rs::jsapi::Value::set(int value) {
    InitValueRef();
    value_.setInt32(value);
}

void rs::jsapi::Value::set(bool value) {
    InitValueRef();
    value_.setBoolean(value);
}

void rs::jsapi::Value::set(double value) {
    InitValueRef();
    value_.setDouble(value);
}

void rs::jsapi::Value::setUndefined() {
    InitValueRef();
    value_.setUndefined();
}

void rs::jsapi::Value::setNull() {
    InitValueRef();
    value_.setNull();
}

std::string rs::jsapi::Value::ToString() const {
    JSString* str = nullptr;
    if (isObject_) {
        JS::RootedValue rv(cx_);
        rv.setObjectOrNull(object_);
        str = JS::ToString(cx_, rv);
    } else {
        str = JS::ToString(cx_, value_);
    }
    
    auto len = JS_GetStringLength(str);
    if (len <= 256) {
        auto chars = reinterpret_cast<char*>(alloca(len));
        JS_EncodeStringToBuffer(cx_, str, chars, len);
        return std::string(chars, len);
    } else {
        std::vector<char> chars(len);
        JS_EncodeStringToBuffer(cx_, str, &chars[0], len);
        return std::string(&chars[0], chars.size());
    }
}

bool rs::jsapi::Value::isString() const { 
    return !isObject_ && value_.isString(); 
}

bool rs::jsapi::Value::isNumber() const { 
    return !isObject_ && value_.isNumber(); 
}

bool rs::jsapi::Value::isInt32() const { 
    return !isObject_ && value_.isInt32(); 
}

bool rs::jsapi::Value::isBoolean() const { 
    return !isObject_ && value_.isBoolean(); 
}

bool rs::jsapi::Value::isObject() const {
    return isObject_ || value_.isObjectOrNull();
}

bool rs::jsapi::Value::isNull() const { 
    return !isObject_ && value_.isNull(); 
}

bool rs::jsapi::Value::isUndefined() const { 
    return !isObject_ && value_.isUndefined(); 
}

bool rs::jsapi::Value::isArray() const {     
    auto isArray = false;
    
    if (isObject_) {
        object_ && JS_IsArrayObject(cx_, object_, &isArray);
    } else {
        value_.toObjectOrNull() != nullptr && JS_IsArrayObject(cx_, value_, &isArray);
    }
    
    return isArray;
}

bool rs::jsapi::Value::isFunction() const { 
    if (isObject_) {
        return object_ && JS_ObjectIsFunction(cx_, object_);
    } else {
        auto obj = value_.toObjectOrNull();
        return obj != nullptr && JS_ObjectIsFunction(cx_, obj);
    }
}

JSString* rs::jsapi::Value::toString() const { 
    if (isObject_) {
        throw ValueCastException();    
    }
    
    return value_.toString(); 
}

double rs::jsapi::Value::toNumber() const { 
    if (isObject_) {
        throw ValueCastException();    
    }
    
    return value_.toNumber(); 
}

int32_t rs::jsapi::Value::toInt32() const { 
    if (isObject_) {
        throw ValueCastException();    
    }
    
    return value_.toInt32(); 
}

bool rs::jsapi::Value::toBoolean() const { 
    if (isObject_) {
        throw ValueCastException();    
    }
    
    return value_.toBoolean(); 
}

JSObject* rs::jsapi::Value::toObject() const { 
    ToObjectRef();
    return object_.get();
}

const JS::HandleValue rs::jsapi::Value::toFunction() const { 
    ToValueRef();
    return value_;
}

bool rs::jsapi::Value::CallFunction(const FunctionArguments& args, bool throwOnError) {
    ToValueRef();
    return Context::Call(*this, args, throwOnError);
}

bool rs::jsapi::Value::CallFunction(const FunctionArguments& args, Value& result, bool throwOnError) {
    ToValueRef();
    return Context::Call(*this, args, result, throwOnError);
}

JSContext* rs::jsapi::Value::getContext() const { 
    return cx_; 
}

void rs::jsapi::Value::InitValueRef() const {
    if (isObject_) {
        object_ = nullptr;
        isObject_ = false;
    }
}

void rs::jsapi::Value::InitObjectRef() const {        
    if (!isObject_) {
        value_.setNull();
        isObject_ = true;
    }
}

void rs::jsapi::Value::ToValueRef() const {
    if (isObject_) {
        JS::RootedValue rv(cx_);
        rv.setObjectOrNull(object_);
        
        InitValueRef();
        
        value_ = rv;
    }
}

void rs::jsapi::Value::ToObjectRef() const {
    if (!isObject_) {
        if (!value_.isObjectOrNull()) {
            throw ValueCastException();
        }
        
        JS::RootedObject ro(cx_, value_.toObjectOrNull());
        InitObjectRef();        
        object_ = ro;
    }
}

std::ostream& operator<<(std::ostream& os, const rs::jsapi::Value& value) {
    if (value.isBoolean()) {
        return os << (value.toBoolean() ? "true" : "false");
    } else if (value.isInt32()) {
        return os << value.toInt32();
    } else if (value.isNumber()) {
        return os << value.toNumber();
    } else {
        return os << value.ToString();
    }
}