#include "global.h"
#include "context.h"
#include "value.h"

bool rs::jsapi::Global::DefineProperty(Context& cx, const char* name, const Value& value, unsigned attrs) {
    if (value.isObject()) {
        return JS_DefineProperty(cx, cx.getGlobal(), name, value.getHandleObject(), attrs, nullptr, nullptr);
    } else {    
        return JS_DefineProperty(cx, cx.getGlobal(), name, value.getHandleValue(), attrs, nullptr, nullptr);
    }
}

bool rs::jsapi::Global::DefineProperty(Context& cx, const char* name, JSNative getter, JSNative setter, unsigned attrs) {
    return JS_DefineProperty(cx, cx.getGlobal(), name, JS::NullHandleValue, 
        attrs | JSPROP_NATIVE_ACCESSORS,
        reinterpret_cast<JSPropertyOp>(getter), reinterpret_cast<JSStrictPropertyOp>(setter));
}

bool rs::jsapi::Global::DefineFunction(Context& cx, const char* name, JSNative func, unsigned attrs) {
    return JS_DefineFunction(cx, cx.getGlobal(), name, func, 0, attrs);
}