#include "dynamic_array.h"
#include "context.h"
#include "value.h"

#include <vector>
#include <cstring>

JSClass rs::jsapi::DynamicArray::class_ = { 
    "rs_jsapi_dynamicarray", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    DynamicArray::Get, DynamicArray::Set, JS_EnumerateStub, JS_ResolveStub, 
    JS_ConvertStub, DynamicArray::Finalize
};

bool rs::jsapi::DynamicArray::Create(Context& cx, GetCallback getter, SetCallback setter, LengthCallback length, FinalizeCallback finalize, Value& array) {
    JS::RootedObject obj(cx, JS_NewObject(cx, &class_, JS::NullPtr(), JS::NullPtr()));
    
    if (obj) {
        JS_DefineProperty(cx, obj, "length", 0, JSPROP_READONLY, DynamicArray::Length, nullptr);
        
        auto state = new DynamicArrayState { getter, setter, length, finalize, 0, nullptr };
        DynamicArray::SetState(obj, state);
        
        array.set(obj);
    }
    
    return obj;
}

bool rs::jsapi::DynamicArray::Get(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
    auto state = DynamicArray::GetState(cx, obj);
    
    if (state != nullptr && state->getter != nullptr && JSID_IS_INT(id)) {
        try {
            auto index = JSID_TO_INT(id);
            Value value(cx);
            value.setUndefined();
            state->getter(index, value);            
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

bool rs::jsapi::DynamicArray::Set(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
    auto state = DynamicArray::GetState(cx, obj);
    
    if (state != nullptr && state->setter != nullptr && JSID_IS_INT(id)) {
        try {
            auto index = JSID_TO_INT(id);
            Value value(cx, vp);                
            state->setter(index, value);    
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

void rs::jsapi::DynamicArray::Finalize(JSFreeOp* fop, JSObject* obj) {
    auto state = GetState(obj);
    if (state != nullptr && state->finalize != nullptr) {
        state->finalize();
    }
    
    SetState(obj, nullptr);
    delete state;    
}

bool rs::jsapi::DynamicArray::Length(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {    
    auto length = 0;
    
    auto state = DynamicArray::GetState(cx, obj);    
    if (state != nullptr && state->length != nullptr) {
        length = state->length();
    }
    
    vp.setInt32(length);
    
    return true;
}

rs::jsapi::DynamicArray::DynamicArrayState* rs::jsapi::DynamicArray::GetState(JSContext* cx, JS::HandleObject obj) {
    auto state = JS_GetInstancePrivate(cx, obj, &DynamicArray::class_, nullptr);
    return reinterpret_cast<DynamicArrayState*>(state);
}

rs::jsapi::DynamicArray::DynamicArrayState* rs::jsapi::DynamicArray::GetState(JSObject* obj) {
    auto state = JS_GetPrivate(obj);
    return reinterpret_cast<DynamicArrayState*>(state);
}

void rs::jsapi::DynamicArray::SetState(JSObject* obj, DynamicArrayState* state) {
    JS_SetPrivate(obj, state);
}

bool rs::jsapi::DynamicArray::SetPrivate(Value& value, uint64_t data, void* ptr) {
    auto set = false;
    if (value.isObject()) {
        auto obj = value.toObject();
        auto klass = JS_GetClass(obj);
        if (klass != nullptr && std::strcmp(klass->name, DynamicArray::class_.name) == 0) {
            auto state = GetState(obj);
            state->data = data;
            state->ptr = ptr;
            set = true;
        }
    }
    
    return set;
}

bool rs::jsapi::DynamicArray::GetPrivate(const Value& value, uint64_t& data, void*& ptr) {
    auto get = false;
    if (value.isObject()) {
        auto obj = value.toObject();
        auto klass = JS_GetClass(obj);
        if (klass != nullptr && std::strcmp(klass->name, DynamicArray::class_.name) == 0) {
            auto state = GetState(obj);
            data = state->data;
            ptr = state->ptr;
            get = true;
        }
    }
    
    return get;
}