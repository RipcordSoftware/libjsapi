#include "object.h"
#include "context.h"
#include "value.h"

#include <vector>

JSClass rs::jsapi::Object::class_ = { 
    "rs_jsapi_object", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    JS_PropertyStub, JS_StrictPropertyStub, JS_EnumerateStub, JS_ResolveStub, 
    JS_ConvertStub, Object::Finalize 
};

bool rs::jsapi::Object::Create(Context& cx, std::initializer_list<const char*> properties, 
        GetCallback getter, SetCallback setter, std::initializer_list<Function> functions, FinalizeCallback finalizer, Value& obj) {
    JS::RootedObject newObj(cx, JS_NewObject(cx, &class_, JS::NullPtr(), JS::NullPtr()));    
    
    if (newObj) {
        for (auto p : properties) {
            JS_DefineProperty(cx, newObj, p, JS::NullHandleValue, JSPROP_ENUMERATE, 
                Object::Get, Object::Set);
        }

        for (auto f : functions) {
            JS_DefineFunction(cx, newObj, f.first, f.second, 0, JSPROP_ENUMERATE);
        }

        auto callbacks = new ClassCallbacks { getter, setter, finalizer };
        Object::SetObjectCallbacks(newObj, callbacks);

        obj.set(newObj);
    }
    
    return newObj;
}

bool rs::jsapi::Object::Get(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
    auto callbacks = Object::GetObjectCallbacks(obj);    
    if (callbacks != nullptr && callbacks->getter != nullptr) {
        Value value(cx, vp);
        
        auto name = JSID_TO_STRING(id);
        char nameBuffer[256];
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        auto status = false;
        
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

bool rs::jsapi::Object::Set(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
    auto callbacks = Object::GetObjectCallbacks(obj);
    if (callbacks != nullptr && callbacks->setter != nullptr) {
        Value value(cx, vp);
        
        char nameBuffer[256];
        auto name = JSID_TO_STRING(id);                
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        
        if (nameLength < sizeof(nameBuffer)) {
            nameBuffer[nameLength] = '\0';    
            return callbacks->setter(nameBuffer, value);    
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

void rs::jsapi::Object::Finalize(JSFreeOp* fop, JSObject* obj) {
    auto callbacks = Object::GetObjectCallbacks(obj);
    if (callbacks != nullptr && callbacks->finalizer != nullptr) {
        callbacks->finalizer();
    }
    
    SetObjectCallbacks(obj, nullptr);
    delete callbacks;    
}

rs::jsapi::Object::ClassCallbacks* rs::jsapi::Object::GetObjectCallbacks(JSObject* obj) {
    auto callbacks = JS_GetPrivate(obj);
    return reinterpret_cast<ClassCallbacks*>(callbacks);
}

void rs::jsapi::Object::SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks) {
    JS_SetPrivate(obj, callbacks);    
}