#include "value.h"
#include "context.h"

#include <vector>

rs::jsapi::Value::Value(Context& cx) : cx_(cx), value_(cx) {
}

rs::jsapi::Value::Value(Context& cx, const char* str) : Value(cx) {
    value_.setString(JS_NewStringCopyZ(cx, str));
}

rs::jsapi::Value::Value(Context& cx, int value): Value(cx) {
    value_.setInt32(value);
}

rs::jsapi::Value::Value(Context& cx, bool value) : Value(cx) {
    value_.setBoolean(value);
}

rs::jsapi::Value::Value(Context& cx, double value) : Value(cx) {
    value_.setNumber(value);
}

rs::jsapi::Value::Value(Context& cx, const JS::RootedObject& obj) : Value(cx) {
    value_.setObjectOrNull(obj);
}

std::string rs::jsapi::Value::ToString() {
    auto str = JS::ToString(cx_, value_);
    std::vector<char> chars(JS_GetStringLength(str));
    JS_EncodeStringToBuffer(cx_, str, &chars[0], chars.size());
    return std::string(&chars[0], chars.size());
}