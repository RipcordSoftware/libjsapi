#include "value.h"
#include "context.h"

#include <vector>

rs::jsapi::Value::Value(JSContext* cx) : cx_(cx), value_(cx) {
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

void rs::jsapi::Value::set(const JS::HandleValue& value) {
    value_.set(value);
}

void rs::jsapi::Value::set(const char* str) {
    value_.setString(JS_NewStringCopyZ(cx_, str));
}

void rs::jsapi::Value::set(const std::string& str) {
    value_.setString(JS_NewStringCopyN(cx_, str.c_str(), str.length()));
}

void rs::jsapi::Value::set(int value) {
    value_.setInt32(value);
}

void rs::jsapi::Value::set(bool value) {
    value_.setBoolean(value);
}

void rs::jsapi::Value::set(double value) {
    value_.setDouble(value);
}

std::string rs::jsapi::Value::ToString() {
    auto str = JS::ToString(cx_, value_);
    std::vector<char> chars(JS_GetStringLength(str));
    JS_EncodeStringToBuffer(cx_, str, &chars[0], chars.size());
    return std::string(&chars[0], chars.size());
}

bool rs::jsapi::Value::isString() { 
    return value_.isString(); 
}

bool rs::jsapi::Value::isNumber() { 
    return value_.isNumber(); 
}

bool rs::jsapi::Value::isInt32() { 
    return value_.isInt32(); 
}

bool rs::jsapi::Value::isBoolean() { 
    return value_.isBoolean(); 
}

bool rs::jsapi::Value::isObject() { 
    return value_.isObject(); 
}

bool rs::jsapi::Value::isNull() { 
    return value_.isNull(); 
}

bool rs::jsapi::Value::isUndefined() { 
    return value_.isUndefined(); 
}

JSString* rs::jsapi::Value::toString() { 
    return value_.toString(); 
}

double rs::jsapi::Value::toNumber() { 
    return value_.toNumber(); 
}

int32_t rs::jsapi::Value::toInt32() { 
    return value_.toInt32(); 
}

bool rs::jsapi::Value::toBoolean() { 
    return value_.toBoolean(); 
}

JSObject* rs::jsapi::Value::toObject() { 
    return value_.toObjectOrNull(); 
}