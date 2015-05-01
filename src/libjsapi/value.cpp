#include "value.h"
#include "context.h"
#include "function_arguments.h"

#include <vector>

rs::jsapi::Value::Value(JSContext* cx) : cx_(cx), isObject_(false) {
    new (&value_) JS::RootedValue(cx);
}

rs::jsapi::Value::~Value() {
    using namespace JS;
    if (isObject_) {
        object_.~RootedObject();
    } else {
        value_.~RootedValue();
    }
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

rs::jsapi::Value::Value(JSContext* cx, const JS::RootedObject& value) : cx_(cx), isObject_(true) {
    new (&object_) JS::RootedObject(cx);
    set(value);
}

rs::jsapi::Value::Value(JSContext* cx, JSObject* value) : cx_(cx), isObject_(true) {
    new (&object_) JS::RootedObject(cx);
    set(value);
}

rs::jsapi::Value::Value(JSContext* cx, JSString* value) : cx_(cx), isObject_(false) {
    new (&value_) JS::RootedValue(cx);
    set(value);
}

rs::jsapi::Value::Value(JSContext* cx, const JS::HandleObject& value) : cx_(cx), isObject_(true) {
    new (&object_) JS::RootedObject(cx);
    set(value);
}

void rs::jsapi::Value::set(const Value& value) {
    *this = value;
}

void rs::jsapi::Value::set(const JS::HandleValue& value) {
    InitValueRef();
    value_.set(value);
}

void rs::jsapi::Value::set(const JS::RootedObject& value) {
    InitObjectRef();
    object_.set(value);
}

void rs::jsapi::Value::set(const JS::HandleObject& value) {
    InitObjectRef();
    object_.set(value);
}

void rs::jsapi::Value::set(JSObject* value) {
    InitObjectRef();
    object_.set(value);
}

void rs::jsapi::Value::set(JSString* value) {
    InitValueRef();
    value_.setString(value);
}

void rs::jsapi::Value::set(const char* str) {
    InitValueRef();
    value_.setString(JS_NewStringCopyZ(cx_, str));
}

void rs::jsapi::Value::set(const std::string& str) {
    InitValueRef();
    value_.setString(JS_NewStringCopyN(cx_, str.c_str(), str.length()));
}

void rs::jsapi::Value::set(int value) {
    InitValueRef();
    value_.setInt32(value);
}

void rs::jsapi::Value::set(bool value) {
    InitValueRef();
    value_.setBoolean(value);
}

void rs::jsapi::Value::set(double value) {
    InitValueRef();
    value_.setDouble(value);
}

void rs::jsapi::Value::setUndefined() {
    InitValueRef();
    value_.setUndefined();
}

void rs::jsapi::Value::setNull() {
    InitValueRef();
    value_.setNull();
}

std::string rs::jsapi::Value::ToString() const {
    JSString* str = nullptr;
    if (isObject_) {
        JS::RootedValue rv(cx_);
        rv.setObjectOrNull(object_);
        str = JS::ToString(cx_, rv);
    } else {
        str = JS::ToString(cx_, value_);
    }
    
    auto len = JS_GetStringLength(str);
    if (len <= 256) {
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
    return !isObject_ && value_.isString(); 
}

bool rs::jsapi::Value::isNumber() const { 
    return !isObject_ && value_.isNumber(); 
}

bool rs::jsapi::Value::isInt32() const { 
    return !isObject_ && value_.isInt32(); 
}

bool rs::jsapi::Value::isBoolean() const { 
    return !isObject_ && value_.isBoolean(); 
}

bool rs::jsapi::Value::isObject() const {
    return isObject_ || value_.isObjectOrNull();
}

bool rs::jsapi::Value::isNull() const { 
    return !isObject_ && value_.isNull(); 
}

bool rs::jsapi::Value::isUndefined() const { 
    return !isObject_ && value_.isUndefined(); 
}

bool rs::jsapi::Value::isArray() const {     
    if (isObject_) {
        return object_ && JS_IsArrayObject(cx_, object_);
    } else {
        return value_.toObjectOrNull() != nullptr && JS_IsArrayObject(cx_, value_);
    }
}

bool rs::jsapi::Value::isFunction() const { 
    if (isObject_) {
        return object_ && JS_ObjectIsFunction(cx_, object_);
    } else {
        auto obj = value_.toObjectOrNull();
        return obj != nullptr && JS_ObjectIsFunction(cx_, obj);
    }
}

JSString* rs::jsapi::Value::toString() const { 
    if (isObject_) {
        throw ValueCastException();    
    }
    
    return value_.toString(); 
}

double rs::jsapi::Value::toNumber() const { 
    if (isObject_) {
        throw ValueCastException();    
    }
    
    return value_.toNumber(); 
}

int32_t rs::jsapi::Value::toInt32() const { 
    if (isObject_) {
        throw ValueCastException();    
    }
    
    return value_.toInt32(); 
}

bool rs::jsapi::Value::toBoolean() const { 
    if (isObject_) {
        throw ValueCastException();    
    }
    
    return value_.toBoolean(); 
}

JSObject* rs::jsapi::Value::toObject() const { 
    ToObjectRef();
    return object_.get();
}

const JS::HandleValue rs::jsapi::Value::toFunction() const { 
    ToValueRef();
    return value_;
}

bool rs::jsapi::Value::CallFunction(const FunctionArguments& args, Value& result) {
    ToValueRef();
    return JS_CallFunctionValue(cx_, JS::NullPtr(), value_, args, result);
}

JSContext* rs::jsapi::Value::getContext() { 
    return cx_; 
}

void rs::jsapi::Value::InitValueRef() const {
    if (isObject_) {
        using namespace JS;
        object_.~RootedObject();
        new (&value_) JS::RootedValue(cx_);
        isObject_ = false;
    }
}

void rs::jsapi::Value::InitObjectRef() const {        
    if (!isObject_) {
        using namespace JS;
        value_.~RootedValue();
        new (&object_) RootedObject(cx_);
        isObject_ = true;
    }
}

void rs::jsapi::Value::ToValueRef() const {
    if (isObject_) {
        JS::RootedValue rv(cx_);
        rv.setObjectOrNull(object_);
        
        InitValueRef();
        
        value_.setObjectOrNull(rv.toObjectOrNull());
    }
}

void rs::jsapi::Value::ToObjectRef() const {
    if (!isObject_) {
        if (!value_.isObjectOrNull()) {
            throw ValueCastException();
        }
        
        JS::RootedObject ro(cx_, value_.toObjectOrNull());        
        InitObjectRef();        
        object_.set(ro);
    }
}

std::ostream& operator<<(std::ostream& os, const rs::jsapi::Value& value) {
    if (value.isBoolean()) {
        return os << (value.toBoolean() ? "true" : "false");
    } else if (value.isInt32()) {
        return os << value.toInt32();
    } else if (value.isNumber()) {
        return os << value.toNumber();
    } else {
        return os << value.ToString();
    }
}