#include "value.h"
#include "context.h"

rs::jsapi::Value::Value(Context& cx) : cx_(cx), value_(cx) {
}
    
// TODO: revise this since it is ugly
std::string rs::jsapi::Value::ToString() {
    if (value_.isString()) {
        auto jsStr = value_.toString();
        auto chars = JS_EncodeString(cx_, jsStr);
        std::string str(chars);
        JS_free(cx_, chars);
        return str;
    } else if (value_.isInt32()) {
        return std::to_string(value_.toInt32());
    } else if (value_.isNumber()) {
        return std::to_string(value_.toNumber());
    } else if (value_.isBoolean()) {
        auto val = value_.toBoolean();
        return val ? "true" : "false";
    } else if (value_.isObject()) {
        return "[object Object]";
    } else if (value_.isNull()) {
        return "null";
    } else {
        return "undefined";
    }     
}