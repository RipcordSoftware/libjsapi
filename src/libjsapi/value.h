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
    Value(JSContext* cx, const JS::HandleObject& obj);
    Value(JSContext* cx, const JS::RootedObject& obj);
    Value(JSContext* cx, JSObject* obj);

    ~Value();    
    
    Value(const Value&) = delete;
    Value& operator=(const Value& rhs) {
        cx_ = rhs.cx_;
        if (rhs.isObject_) {
            set(rhs.object_);
        } else {
            set(rhs.value_);
        }
    }
    
    operator const JS::Value&() const { ToValueRef(); return value_.get(); }
    operator const JS::HandleValue() const { ToValueRef(); return value_; }
    operator const JS::HandleObject() const { ToObjectRef(); return object_; }
    operator JS::MutableHandleValue() { InitValueRef(); return &value_; }
    operator JS::MutableHandleObject() { InitObjectRef(); return &object_; }
    
    bool operator !() const { return isObject() ? toObject() == nullptr : false; }
    
    JSContext* getContext();
    
    void set(const char* str);
    void set(const std::string& str);
    void set(int value);
    void set(bool value);
    void set(double value);
    void set(const JS::HandleValue& value);
    void set(const JS::HandleObject& value);
    void set(const JS::RootedObject& value);
    void set(JSObject* value);
    void set(const Value& value);
    void setUndefined();
    void setNull();
    
    bool isString() const;
    bool isNumber() const;
    bool isInt32() const;
    bool isBoolean() const;
    bool isObject() const;
    bool isNull() const;
    bool isUndefined() const;
    bool isArray() const;
    
    JSString* toString() const;
    double toNumber() const;
    int32_t toInt32() const;
    bool toBoolean() const;
    JSObject* toObject() const;

    std::string ToString() const;

protected:
    JSContext* cx_;
    mutable bool isObject_;
    union {
        mutable JS::RootedValue value_;
        mutable JS::RootedObject object_;
    };
    
    void InitValueRef() const;
    void InitObjectRef() const;
    void ToValueRef() const;
    void ToObjectRef() const;
};

}}

std::ostream& operator<<(std::ostream& os, const rs::jsapi::Value& value);
    
#endif	/* RS_JSAPI_VALUE_H */

