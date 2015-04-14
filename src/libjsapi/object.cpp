#include "object.h"

#include "context.h"

rs::jsapi::Object::Object(Context& cx) : Object(cx, JS_NewObject(cx, nullptr, JS::NullPtr(), JS::NullPtr())) {    
}

rs::jsapi::Object::Object(Context& cx, JSObject* obj) : cx_(cx), obj_(cx, obj) {    
}

bool rs::jsapi::Object::DefineProperty(const char* name, JSNative getter, JSNative setter, unsigned attrs) {
    return JS_DefineProperty(cx_, obj_, name, JS::NullHandleValue, 
        attrs | JSPROP_NATIVE_ACCESSORS,
        reinterpret_cast<JSPropertyOp>(getter), reinterpret_cast<JSStrictPropertyOp>(setter));
}

bool rs::jsapi::Object::DefineGlobalProperty(Context& cx, const char* name, JSNative getter, JSNative setter, unsigned attrs) {
    return JS_DefineProperty(cx, cx.getGlobal(), name, JS::NullHandleValue, 
        attrs | JSPROP_NATIVE_ACCESSORS,
        reinterpret_cast<JSPropertyOp>(getter), reinterpret_cast<JSStrictPropertyOp>(setter));
}

bool rs::jsapi::Object::DefineFunction(const char* name, JSNative func, unsigned attrs) {
    return JS_DefineFunction(cx_, obj_, name, func, 0, attrs);
}

bool rs::jsapi::Object::DefineGlobalFunction(Context& cx, const char* name, JSNative func, unsigned attrs) {
    return JS_DefineFunction(cx, cx.getGlobal(), name, func, 0, attrs);
}