#ifndef RS_JSAPI_VALUE_H
#define RS_JSAPI_VALUE_H

#include <string>
#include <iostream>

#include <jsapi.h>

namespace rs {
namespace jsapi {

class Context;
class Value;
    
class Value {
public:
    Value(JSContext* cx);
    Value(JSContext* cx, const char* str);
    Value(JSContext* cx, const std::string& str);
    Value(JSContext* cx, int value);
    Value(JSContext* cx, bool value);
    Value(JSContext* cx, double value);
    Value(JSContext* cx, const JS::HandleValue& obj);
    
    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;    
    
    operator const JS::Value&() const { return value_.get(); }
    operator const JS::HandleValue() const { return value_; }
    operator JS::MutableHandleValue() { return &value_; }
    operator JSContext*() { return cx_; }
    
    JSContext* getContext();
    
    void set(const char* str);
    void set(const std::string& str);
    void set(int value);
    void set(bool value);
    void set(double value);
    void set(const JS::HandleValue& value);
    void setUndefined();
    void setNull();
    
    bool isString() const;
    bool isNumber() const;
    bool isInt32() const;
    bool isBoolean() const;
    bool isObject() const;
    bool isNull() const;
    bool isUndefined() const;
    
    JSString* toString() const;
    double toNumber() const;
    int32_t toInt32() const;
    bool toBoolean() const;
    JSObject* toObject() const;

    std::string ToString() const;

protected:
    JSContext* cx_;
    JS::RootedValue value_;    
};

}}

std::ostream& operator<<(std::ostream& os, const rs::jsapi::Value& value);
    
#endif	/* RS_JSAPI_VALUE_H */

