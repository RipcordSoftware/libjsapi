#include "dynamic_object.h"
#include "context.h"
#include "value.h"

#include <vector>

JSClass rs::jsapi::DynamicObject::class_ = { 
    "rs_jsapi_dynamicobject", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    DynamicObject::GetCallback, DynamicObject::SetCallback, JS_EnumerateStub, JS_ResolveStub, 
    JS_ConvertStub, DynamicObject::FinalizeCallback 
};

bool rs::jsapi::DynamicObject::Create(Context& cx, Getter getter, DynamicObject::Setter setter, JS::RootedObject& obj) {
    obj = JS::RootedObject(cx, JS_NewObject(cx, &class_, JS::NullPtr(), JS::NullPtr()));
    
    if (obj) {
        auto callbacks = new ClassCallbacks { getter, setter };
        DynamicObject::SetObjectCallbacks(obj, callbacks);        
    }
    
    return obj;
}

bool rs::jsapi::DynamicObject::GetCallback(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
    auto callbacks = DynamicObject::GetObjectCallbacks(obj);
    
    if (callbacks != nullptr && callbacks->getter != nullptr) {
        Value value(cx, vp);
        
        auto name = JSID_TO_STRING(id);
        char nameBuffer[256];
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        bool status = false;
        
        if (nameLength < sizeof(nameBuffer)) {
            nameBuffer[nameLength] = '\0';    
            status = callbacks->getter(nameBuffer, value);    
        } else {
            std::vector<char> nameVector(nameLength + 1);
            nameLength = JS_EncodeStringToBuffer(cx, name, &nameVector[0], nameVector.size() - 1);
            nameVector[nameLength] = '\0';
            status = callbacks->getter(&nameVector[0], value);
        }
        
        if (status) {
            vp.set(value);
        }
        return status;
    } else {
        vp.setUndefined();
        return true;
    }
}

bool rs::jsapi::DynamicObject::SetCallback(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
    auto callbacks = DynamicObject::GetObjectCallbacks(obj);
    
    if (callbacks != nullptr && callbacks->setter != nullptr) {
        Value value(cx, vp);
        
        auto name = JSID_TO_STRING(id);
        char nameBuffer[256];
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        
        if (nameLength < sizeof(nameBuffer)) {
            nameBuffer[nameLength] = '\0';    
            return callbacks->setter( nameBuffer, value);    
        } else {
            std::vector<char> nameVector(nameLength + 1);
            nameLength = JS_EncodeStringToBuffer(cx, name, &nameVector[0], nameVector.size() - 1);
            nameVector[nameLength] = '\0';
            return callbacks->setter(&nameVector[0], value);
        }    
    } else {
        // TODO: what will this do to the JS?
        vp.setUndefined();
        return true;
    }
}

void rs::jsapi::DynamicObject::FinalizeCallback(JSFreeOp* fop, JSObject* obj) {
    delete GetObjectCallbacks(obj);
    SetObjectCallbacks(obj, nullptr);
}

rs::jsapi::DynamicObject::ClassCallbacks* rs::jsapi::DynamicObject::GetObjectCallbacks(JSObject* obj) {
    auto callbacks = JS_GetPrivate(obj);
    return reinterpret_cast<ClassCallbacks*>(callbacks);
}

void rs::jsapi::DynamicObject::SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks) {
    JS_SetPrivate(obj, callbacks);    
}