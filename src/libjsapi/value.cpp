#include "value.h"
#include "context.h"

#include <vector>

rs::jsapi::Value::Value(Context& cx) : cx_(cx), value_(cx) {
}

rs::jsapi::Value::Value(Context& cx, const char* str) : Value(cx) {
    set(str);
}

rs::jsapi::Value::Value(Context& cx, const std::string& str) : Value(cx) {
    set(str);
}

rs::jsapi::Value::Value(Context& cx, int value): Value(cx) {
    set(value);
}

rs::jsapi::Value::Value(Context& cx, bool value) : Value(cx) {
    set(value);
}

rs::jsapi::Value::Value(Context& cx, double value) : Value(cx) {
    set(value);
}

rs::jsapi::Value::Value(Context& cx, const JS::HandleValue& value) : Value(cx) {
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