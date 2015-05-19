#include "dynamic_object.h"
#include "context.h"
#include "value.h"

#include <vector>
#include <cstring>

JSClass rs::jsapi::DynamicObject::class_ = { 
    "rs_jsapi_dynamicobject", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    DynamicObject::Get, DynamicObject::Set, DynamicObject::Enumerate, JS_ResolveStub, 
    JS_ConvertStub, DynamicObject::Finalize 
};

bool rs::jsapi::DynamicObject::Create(Context& cx, GetCallback getter, DynamicObject::SetCallback setter, EnumeratorCallback enumerator, FinalizeCallback finalize, Value& obj) {
    JSAutoRequest ar(cx);    
    JS::RootedObject newObj(cx, JS_NewObject(cx, &class_, JS::NullPtr(), JS::NullPtr()));
    
    if (newObj) {
        auto state = new DynamicObjectState { getter, setter, enumerator, finalize, 0, nullptr };
        DynamicObject::SetState(newObj, state);
        
        obj.set(newObj);
    }
    
    return newObj;
}

bool rs::jsapi::DynamicObject::Get(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
    JSAutoRequest ar(cx);    
    auto state = DynamicObject::GetState(cx, obj);
    
    if (state != nullptr && state->getter != nullptr) {
        Value value(cx);
        
        auto name = JSID_TO_STRING(id);
        char nameBuffer[256];
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);        
        
        try {
            value.setUndefined();
            
            if (nameLength < sizeof(nameBuffer)) {
                nameBuffer[nameLength] = '\0';    
                state->getter(nameBuffer, value);    
            } else {
                std::vector<char> nameVector(nameLength + 1);
                nameLength = JS_EncodeStringToBuffer(cx, name, &nameVector[0], nameVector.size() - 1);
                nameVector[nameLength] = '\0';
                state->getter(&nameVector[0], value);
            }

            vp.set(value);
            return true;
        } catch (const std::exception& ex) {
            JS_ReportError(cx, ex.what());
            return false;
        }
    } else {
        vp.setUndefined();
        return true;
    }
}

bool rs::jsapi::DynamicObject::Set(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
    JSAutoRequest ar(cx);    
    auto state = DynamicObject::GetState(cx, obj);
    
    if (state != nullptr && state->setter != nullptr) {
        Value value(cx, vp);
        
        auto name = JSID_TO_STRING(id);
        char nameBuffer[256];
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        
        try {
            if (nameLength < sizeof(nameBuffer)) {
                nameBuffer[nameLength] = '\0';    
                state->setter( nameBuffer, value);    
            } else {
                std::vector<char> nameVector(nameLength + 1);
                nameLength = JS_EncodeStringToBuffer(cx, name, &nameVector[0], nameVector.size() - 1);
                nameVector[nameLength] = '\0';
                state->setter(&nameVector[0], value);
            }

            return true;
        } catch (const std::exception& ex) {
            JS_ReportError(cx, ex.what());
            return false;
        }
    } else {
        // TODO: what will this do to the JS?
        vp.setUndefined();
        return true;
    }
}

bool rs::jsapi::DynamicObject::Enumerate(JSContext* cx, JS::HandleObject obj) {
    JSAutoRequest ar(cx);

    auto status = true;
    auto state = DynamicObject::GetState(cx, obj);
    if (state != nullptr && state->enumerator != nullptr) {
        std::vector<std::string> props;
        std::vector<std::pair<std::string, JSNative>> funcs;
        status = state->enumerator(props, funcs);
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
    auto state = DynamicObject::GetState(obj);
    if (state != nullptr && state->finalize != nullptr) {
        state->finalize();
    }
    
    SetState(obj, nullptr);
    delete state;    
}

rs::jsapi::DynamicObject::DynamicObjectState* rs::jsapi::DynamicObject::GetState(JSContext* cx, JS::HandleObject obj) {
    JSAutoRequest ar(cx);    
    auto state = JS_GetInstancePrivate(cx, obj, &DynamicObject::class_, nullptr);
    return reinterpret_cast<DynamicObjectState*>(state);
}

rs::jsapi::DynamicObject::DynamicObjectState* rs::jsapi::DynamicObject::GetState(JSObject* obj) {
    auto state = JS_GetPrivate(obj);
    return reinterpret_cast<DynamicObjectState*>(state);
}

void rs::jsapi::DynamicObject::SetState(JSObject* obj, DynamicObjectState* state) {
    JS_SetPrivate(obj, state);    
}

bool rs::jsapi::DynamicObject::SetPrivate(Value& value, uint64_t data, void* ptr) {
    auto set = false;
    if (value.isObject()) {
        auto obj = value.toObject();
        auto klass = JS_GetClass(obj);
        if (klass != nullptr && std::strcmp(klass->name, DynamicObject::class_.name) == 0) {
            auto state = GetState(obj);
            state->data = data;
            state->ptr = ptr;
            set = true;
        }
    }
    
    return set;
}

bool rs::jsapi::DynamicObject::GetPrivate(const Value& value, uint64_t& data, void*& ptr) {
    auto get = false;
    if (value.isObject()) {
        auto obj = value.toObject();
        auto klass = JS_GetClass(obj);
        if (klass != nullptr && std::strcmp(klass->name, DynamicObject::class_.name) == 0) {
            auto state = GetState(obj);
            data = state->data;
            ptr = state->ptr;
            get = true;
        }
    }
    
    return get;
}