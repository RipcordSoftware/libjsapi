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
    
    bool isString();
    bool isNumber();
    bool isInt32();
    bool isBoolean();
    bool isObject();
    bool isNull();
    bool isUndefined();
    
    JSString* toString();
    double toNumber();
    int32_t toInt32();
    bool toBoolean();
    JSObject* toObject();

    std::string ToString();

protected:
    JSContext* cx_;
    JS::RootedValue value_;    
};

}}    
    
#endif	/* RS_JSAPI_VALUE_H */

