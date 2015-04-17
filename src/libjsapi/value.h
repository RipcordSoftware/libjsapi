#ifndef RS_JSAPI_VALUE_H
#define RS_JSAPI_VALUE_H

#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {

class Context;
class Value;
    
class Value {
public:
    Value(Context& cx);
    Value(Context& cx, const char* str);
    Value(Context& cx, int value);
    Value(Context& cx, bool value);
    Value(Context& cx, double value);
    Value(Context& cx, const JS::RootedObject& obj);
    
    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;    
    
    operator const JS::Value&() const { return value_.get(); }
    
    bool isString() { return value_.isString(); }
    bool isNumber() { return value_.isNumber(); }
    bool isInt32() { return value_.isInt32(); }
    bool isBoolean() { return value_.isBoolean(); }
    bool isObject() { return value_.isObject(); }
    bool isNull() { return value_.isNull(); }
    bool isUndefined() { return value_.isUndefined(); }
    
    JSString* toString() { return value_.toString(); }
    double toNumber() { return value_.toNumber(); }
    int32_t toInt32() { return value_.toInt32(); }
    bool toBoolean() { return value_.toBoolean(); }
    JSObject* toObject() { return value_.toObjectOrNull(); }

    std::string ToString();

protected:
    Context& cx_;
    JS::RootedValue value_;    
};

}}    
    
#endif	/* RS_JSAPI_VALUE_H */

