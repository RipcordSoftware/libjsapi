#include "dynamic_object.h"
#include "context.h"
#include "value.h"

#include <vector>

JSClass rs::jsapi::DynamicObject::class_ = { 
    "rs_jsapi_dynamicobject", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    DynamicObject::Get, DynamicObject::Set, DynamicObject::Enumerate, JS_ResolveStub, 
    JS_ConvertStub, DynamicObject::Finalize 
};

bool rs::jsapi::DynamicObject::Create(Context& cx, GetCallback getter, DynamicObject::SetCallback setter, EnumeratorCallback enumerator, FinalizeCallback finalize, Value& obj) {
    JS::RootedObject newObj(cx, JS_NewObject(cx, &class_, JS::NullPtr(), JS::NullPtr()));
    
    if (newObj) {
        auto callbacks = new ClassCallbacks { getter, setter, enumerator, finalize };
        DynamicObject::SetObjectCallbacks(newObj, callbacks);
        
        obj.set(newObj);
    }
    
    return newObj;
}

bool rs::jsapi::DynamicObject::Get(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
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

bool rs::jsapi::DynamicObject::Set(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
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

bool rs::jsapi::DynamicObject::Enumerate(JSContext* cx, JS::HandleObject obj) {
    auto status = true;
    auto callbacks = DynamicObject::GetObjectCallbacks(obj);
    if (callbacks != nullptr && callbacks->enumerator != nullptr) {
        std::vector<std::string> props;
        std::vector<std::pair<std::string, JSNative>> funcs;
        status = callbacks->enumerator(props, funcs);
        if (status) {
            for (auto p : props) {
                JS_DefineProperty(cx, obj, p.c_str(), JS::NullHandleValue, JSPROP_ENUMERATE, 
                    DynamicObject::Get, DynamicObject::Set);
            }
            
            for (auto f : funcs) {
                JS_DefineFunction(cx, obj, f.first.c_str(), f.second, 0, JSPROP_ENUMERATE);
            }
        }
    }
    return status;
}

void rs::jsapi::DynamicObject::Finalize(JSFreeOp* fop, JSObject* obj) {
    auto callbacks = DynamicObject::GetObjectCallbacks(obj);
    if (callbacks != nullptr && callbacks->finalize != nullptr) {
        callbacks->finalize();
    }
    
    SetObjectCallbacks(obj, nullptr);
    delete callbacks;    
}

rs::jsapi::DynamicObject::ClassCallbacks* rs::jsapi::DynamicObject::GetObjectCallbacks(JSObject* obj) {
    auto callbacks = JS_GetPrivate(obj);
    return reinterpret_cast<ClassCallbacks*>(callbacks);
}

void rs::jsapi::DynamicObject::SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks) {
    JS_SetPrivate(obj, callbacks);    
}