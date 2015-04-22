#include "dynamic_array.h"
#include "context.h"
#include "value.h"

#include <vector>

JSClass rs::jsapi::DynamicArray::class_ = { 
    "rs_jsapi_dynamicarray", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    DynamicArray::Get, DynamicArray::Set, JS_EnumerateStub, JS_ResolveStub, 
    JS_ConvertStub, DynamicArray::Finalize
};

bool rs::jsapi::DynamicArray::Create(Context& cx, GetCallback getter, SetCallback setter, LengthCallback length, FinalizeCallback finalize, Value& array) {
    JS::RootedObject obj(cx, JS_NewObject(cx, &class_, JS::NullPtr(), JS::NullPtr()));
    
    if (obj) {
        JS_DefineProperty(cx, obj, "length", 0, JSPROP_ENUMERATE | JSPROP_READONLY, DynamicArray::Length, nullptr);
        
        auto callbacks = new ClassCallbacks { getter, setter, length, finalize };
        DynamicArray::SetObjectCallbacks(obj, callbacks);
        
        array.set(obj);
    }
    
    return obj;
}

bool rs::jsapi::DynamicArray::Get(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
    auto callbacks = DynamicArray::GetObjectCallbacks(obj);
    
    if (callbacks != nullptr && callbacks->getter != nullptr) {                
        auto index = JSID_TO_INT(id);
        Value value(cx, vp);
        bool status = callbacks->getter(index, value);        
        if (status) {
            vp.set(value);
        }
        return status;
    } else {
        vp.setUndefined();
        return true;
    }
}

bool rs::jsapi::DynamicArray::Set(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
    auto callbacks = DynamicArray::GetObjectCallbacks(obj);
    
    if (callbacks != nullptr && callbacks->setter != nullptr) {
        Value value(cx, vp);        
        auto index = JSID_TO_INT(id);
        return callbacks->setter(index, value);    
    } else {
        // TODO: what will this do to the JS?
        vp.setUndefined();
        return true;
    }
}

void rs::jsapi::DynamicArray::Finalize(JSFreeOp* fop, JSObject* obj) {
    auto callbacks = GetObjectCallbacks(obj);
    if (callbacks != nullptr && callbacks->finalize != nullptr) {
        callbacks->finalize();
    }
    
    SetObjectCallbacks(obj, nullptr);
    delete callbacks;    
}

bool rs::jsapi::DynamicArray::Length(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {    
    auto length = 0;
    
    auto callbacks = DynamicArray::GetObjectCallbacks(obj);    
    if (callbacks != nullptr && callbacks->length != nullptr) {
        length = callbacks->length();
    }
    
    vp.setInt32(length);
    
    return true;
}

rs::jsapi::DynamicArray::ClassCallbacks* rs::jsapi::DynamicArray::GetObjectCallbacks(JSObject* obj) {
    auto callbacks = JS_GetPrivate(obj);
    return reinterpret_cast<ClassCallbacks*>(callbacks);
}

void rs::jsapi::DynamicArray::SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks) {
    JS_SetPrivate(obj, callbacks);    
}