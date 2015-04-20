#include "object.h"
#include "context.h"
#include "value.h"

#include <vector>

JSClass rs::jsapi::Object::class_ = { 
    "rs_jsapi_object", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    JS_PropertyStub, JS_StrictPropertyStub, JS_EnumerateStub, JS_ResolveStub, 
    JS_ConvertStub, Object::FinalizeCallback 
};

bool rs::jsapi::Object::Create(Context& cx, std::initializer_list<const char*> properties, 
        Getter getter, Setter setter, std::initializer_list<Function> functions, JS::RootedObject& obj) {
    obj = JS::RootedObject(cx, JS_NewObject(cx, &class_, JS::NullPtr(), JS::NullPtr()));    
    
    if (obj) {
        for (auto p : properties) {
            JS_DefineProperty(cx, obj, p, JS::NullHandleValue, JSPROP_ENUMERATE, 
                Object::GetCallback, Object::SetCallback);
        }

        for (auto f : functions) {
            auto fn = JS_DefineFunction(cx, obj, f.first, f.second, 0, JSPROP_ENUMERATE);
        }

        auto callbacks = new ClassCallbacks { getter, setter };
        Object::SetObjectCallbacks(obj, callbacks);
    }
    
    return obj;
}

bool rs::jsapi::Object::GetCallback(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
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

bool rs::jsapi::Object::SetCallback(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
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

void rs::jsapi::Object::FinalizeCallback(JSFreeOp* fop, JSObject* obj) {
    delete GetObjectCallbacks(obj);
    SetObjectCallbacks(obj, nullptr);
}

rs::jsapi::Object::ClassCallbacks* rs::jsapi::Object::GetObjectCallbacks(JSObject* obj) {
    auto callbacks = JS_GetPrivate(obj);
    return reinterpret_cast<ClassCallbacks*>(callbacks);
}

void rs::jsapi::Object::SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks) {
    JS_SetPrivate(obj, callbacks);    
}