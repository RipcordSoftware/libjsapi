#include "object.h"
#include "context.h"

#include <vector>

bool rs::jsapi::Object::Create(Context& cx, std::initializer_list<const char*> properties, 
        Getter getter, Setter setter, std::initializer_list<Function> functions, JS::RootedObject& obj) {
    obj = JS::RootedObject(cx, JS_NewObject(cx, nullptr, JS::NullPtr(), JS::NullPtr()));    
    
    for (auto p : properties) {
        JS_DefineProperty(cx, obj, p, JS::NullHandleValue, 
            JSPROP_SHARED | JSPROP_ENUMERATE, 
            Object::GetCallback, Object::SetCallback);
    }
    
    for (auto f : functions) {
        JS_DefineFunction(cx, obj, f.first, f.second, 0, JSPROP_SHARED | JSPROP_ENUMERATE);
    }
    
    SetGetter(cx, obj, getter);
    SetSetter(cx, obj, setter);    
}

bool rs::jsapi::Object::GetCallback(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
    auto getter = GetGetter(cx, obj);    
    if (getter != nullptr) {
        auto name = JSID_TO_STRING(id);
        char nameBuffer[256];
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        if (nameLength < sizeof(nameBuffer)) {
            nameBuffer[nameLength] = '\0';    
            return getter(cx, nameBuffer, vp);    
        } else {
            std::vector<char> nameVector(nameLength + 1);
            nameLength = JS_EncodeStringToBuffer(cx, name, &nameVector[0], nameVector.size() - 1);
            nameVector[nameLength] = '\0';
            return getter(cx, &nameVector[0], vp);
        }
    } else {
        vp.setUndefined();
        return true;
    }
}

bool rs::jsapi::Object::SetCallback(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
    auto setter = GetSetter(cx, obj);
    if (setter != nullptr) {
        auto name = JSID_TO_STRING(id);
        char nameBuffer[256];
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        if (nameLength < sizeof(nameBuffer)) {
            nameBuffer[nameLength] = '\0';    
            return setter(cx, nameBuffer, vp);    
        } else {
            std::vector<char> nameVector(nameLength + 1);
            nameLength = JS_EncodeStringToBuffer(cx, name, &nameVector[0], nameVector.size() - 1);
            nameVector[nameLength] = '\0';
            return setter(cx, &nameVector[0], vp);
        }    
    } else {
        // TODO: what will this do to the JS?
        vp.setUndefined();
        return true;
    }
}

void rs::jsapi::Object::SetGetter(JSContext* cx, JS::HandleObject obj, Getter getter) {
    uint32_t lo = (uint32_t)((uint64_t)getter);
    uint32_t hi = (uint32_t)(((uint64_t)getter) >> 32);
    JS_DefineProperty(cx, obj, "__rs_jsapi_object_getter_lo", lo, JSPROP_READONLY);
    JS_DefineProperty(cx, obj, "__rs_jsapi_object_getter_hi", hi, JSPROP_READONLY);
}

rs::jsapi::Object::Getter rs::jsapi::Object::GetGetter(JSContext* cx, JS::HandleObject obj) {
    JS::RootedValue rv(cx);
    JS::MutableHandleValue value(&rv);
    JS_GetProperty(cx, obj, "__rs_jsapi_object_getter_hi", value);
    
    int64_t ptr = value.toPrivateUint32();
    ptr <<= 32;
    
    JS_GetProperty(cx, obj, "__rs_jsapi_object_getter_lo", value);
    ptr |= value.toPrivateUint32();
    
    return reinterpret_cast<Getter>(ptr);
}

void rs::jsapi::Object::SetSetter(JSContext* cx, JS::HandleObject obj, Setter setter) {
    uint32_t lo = (uint32_t)((uint64_t)setter);
    uint32_t hi = (uint32_t)(((uint64_t)setter) >> 32);
    JS_DefineProperty(cx, obj, "__rs_jsapi_object_setter_lo", lo, JSPROP_READONLY);
    JS_DefineProperty(cx, obj, "__rs_jsapi_object_setter_hi", hi, JSPROP_READONLY);
}

rs::jsapi::Object::Setter rs::jsapi::Object::GetSetter(JSContext* cx, JS::HandleObject obj) {
    JS::RootedValue rv(cx);
    JS::MutableHandleValue value(&rv);
    JS_GetProperty(cx, obj, "__rs_jsapi_object_setter_hi", value);    
    
    int64_t ptr = value.toPrivateUint32();
    ptr <<= 32;
    
    JS_GetProperty(cx, obj, "__rs_jsapi_object_setter_lo", value);
    ptr |= value.toPrivateUint32();
    
    return reinterpret_cast<Setter>(ptr);
}