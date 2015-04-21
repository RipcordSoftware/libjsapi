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

void rs::jsapi::Value::setUndefined() {
    value_.setUndefined();
}

void rs::jsapi::Value::setNull() {
    value_.setNull();
}

std::string rs::jsapi::Value::ToString() const {
    auto str = JS::ToString(cx_, value_);
    auto len = JS_GetStringLength(str);
    if (len <= 64) {
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
    return value_.isString(); 
}

bool rs::jsapi::Value::isNumber() const { 
    return value_.isNumber(); 
}

bool rs::jsapi::Value::isInt32() const { 
    return value_.isInt32(); 
}

bool rs::jsapi::Value::isBoolean() const { 
    return value_.isBoolean(); 
}

bool rs::jsapi::Value::isObject() const { 
    return value_.isObject(); 
}

bool rs::jsapi::Value::isNull() const { 
    return value_.isNull(); 
}

bool rs::jsapi::Value::isUndefined() const { 
    return value_.isUndefined(); 
}

JSString* rs::jsapi::Value::toString() const { 
    return value_.toString(); 
}

double rs::jsapi::Value::toNumber() const { 
    return value_.toNumber(); 
}

int32_t rs::jsapi::Value::toInt32() const { 
    return value_.toInt32(); 
}

bool rs::jsapi::Value::toBoolean() const { 
    return value_.toBoolean(); 
}

JSObject* rs::jsapi::Value::toObject() const { 
    return value_.toObjectOrNull(); 
}

JSContext* rs::jsapi::Value::getContext() { 
    return cx_; 
}

std::ostream& operator<<(std::ostream& os, const rs::jsapi::Value& value) {
    if (value.isBoolean()) {
        return os << value.toBoolean();
    } else if (value.isInt32()) {
        return os << value.toInt32();
    } else if (value.isNumber()) {
        return os << value.toNumber();
    } else {
        return os << value.ToString();
    }
}