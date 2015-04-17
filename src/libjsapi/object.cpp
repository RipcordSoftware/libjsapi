#include "object.h"

#include "context.h"

rs::jsapi::Object::Object(Context& cx) : cx_(cx), obj_(cx, JS_NewObject(cx, nullptr, JS::NullPtr(), JS::NullPtr())) {    
}

bool rs::jsapi::Object::DefineProperty(const char* name, JSNative getter, JSNative setter, unsigned attrs) {
    return JS_DefineProperty(cx_, obj_, name, JS::NullHandleValue, 
        attrs | JSPROP_NATIVE_ACCESSORS,
        reinterpret_cast<JSPropertyOp>(getter), reinterpret_cast<JSStrictPropertyOp>(setter));
}

bool rs::jsapi::Object::DefineFunction(const char* name, JSNative func, unsigned attrs) {
    return JS_DefineFunction(cx_, obj_, name, func, 0, attrs);
}