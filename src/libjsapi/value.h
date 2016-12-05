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

#ifndef RS_JSAPI_VALUE_H
#define RS_JSAPI_VALUE_H

#include <string>
#include <iostream>

#include <jsapi.h>

#include "context_state.h"

namespace rs {
namespace jsapi {

class FunctionArguments;
    
class Value final {
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
    Value(JSContext* cx, JSString* obj);

    ~Value();    
    
    Value(const Value& rhs);

    Value& operator=(const Value& rhs) {
        cx_ = rhs.cx_;
        if (rhs.isObject_) {
            InitObjectRef();
            object_ = rhs.object_;
        } else {
            InitValueRef();
            value_ = rhs.value_;
        }
        
        return *this;
    }
    
    Value& operator=(int value) { set(value); return *this; }
    Value& operator=(double value) { set(value); return *this; }
    Value& operator=(bool value) { set(value); return *this; }
    Value& operator=(const char* str) { set(str); return *this; }
    Value& operator=(const std::string& str) { set(str); return *this; }
    Value& operator=(const JS::HandleValue& value) { set(value); return *this; }
    Value& operator=(const JS::HandleObject& value) { set(value); return *this; }
    Value& operator=(const JS::RootedObject& value) { set(value); return *this; }
    Value& operator=(const JSObject* value) { set(value); return *this; }
    Value& operator=(const JSString* value) { set(value); return *this; }
    
    operator const JS::Value&() const { ToValueRef(); return value_.get(); }
    operator const JS::HandleValue() const { ToValueRef(); return value_; }
    operator const JS::HandleObject() const { ToObjectRef(); return object_; }
    operator JS::MutableHandleValue() { InitValueRef(); return &value_; }
    operator JS::MutableHandleObject() { InitObjectRef(); return &object_; }
    
    bool operator !() const { return isObject() ? toObject() == nullptr : false; }
    
    JSContext* getContext() const;
    const JS::HandleValue getHandleValue() const { ToValueRef(); return value_; }
    const JS::HandleObject getHandleObject() const { ToObjectRef(); return object_; }
    
    void set(const char* str);
    void set(const std::string& str);
    void set(int value);
    void set(bool value);
    void set(double value);
    void set(const JS::HandleValue& value);
    void set(const JS::HandleObject& value);
    void set(const JS::RootedObject& value);
    void set(JSObject* value);
    void set(JSString* value);
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
    bool isFunction() const;
    
    JSString* toString() const;
    double toNumber() const;
    int32_t toInt32() const;
    bool toBoolean() const;
    JSObject* toObject() const;
    const JS::HandleValue toFunction() const;
    
    bool CallFunction(const FunctionArguments&, bool throwOnError = true);
    bool CallFunction(const FunctionArguments&, Value&, bool throwOnError = true);

    std::string ToString() const;

protected:
    mutable JSContext* cx_;
    
    mutable bool isObject_;    
    mutable JS::PersistentRootedValue value_;
    mutable JS::PersistentRootedObject object_;    
    
    void InitValueRef() const;
    void InitObjectRef() const;
    void ToValueRef() const;
    void ToObjectRef() const;
};

}}

std::ostream& operator<<(std::ostream& os, const rs::jsapi::Value& value);
    
#endif	/* RS_JSAPI_VALUE_H */
