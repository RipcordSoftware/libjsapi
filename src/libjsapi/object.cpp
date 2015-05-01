#include "object.h"
#include "context.h"
#include "value.h"

#include <cstring>

JSClass rs::jsapi::Object::class_ = { 
    "rs_jsapi_object", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    JS_PropertyStub, JS_StrictPropertyStub, JS_EnumerateStub, JS_ResolveStub, 
    JS_ConvertStub, Object::Finalize 
};

bool rs::jsapi::Object::Create(Context& cx, const std::vector<const char*>& properties,
        GetCallback getter, SetCallback setter, const std::vector<std::pair<const char*, FunctionCallback>>& functions,
        FinalizeCallback finalizer, Value& obj) {
    JS::RootedObject newObj(cx, JS_NewObject(cx, &class_, JS::NullPtr(), JS::NullPtr()));    
    
    if (newObj) {
        auto state = new ObjectState { getter, setter, finalizer, Functions(), 0, nullptr };
        
        for (auto p : properties) {
            JS_DefineProperty(cx, newObj, p, JS::NullHandleValue, JSPROP_ENUMERATE, 
                Object::Get, Object::Set);
        }

        for (auto f : functions) {
            JS_DefineFunction(cx, newObj, f.first, Object::CallFunction, 0, JSPROP_ENUMERATE);
            state->functions.emplace(f.first, f.second);
        }
        
        Object::SetState(newObj, state);

        obj.set(newObj);
    }
    
    return newObj;
}

bool rs::jsapi::Object::Get(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
    auto state = Object::GetState(obj);    
    if (state != nullptr && state->getter != nullptr) {
        Value value(cx, vp);
        
        auto name = JSID_TO_STRING(id);
        char nameBuffer[256];
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        auto status = false;
        
        if (nameLength < sizeof(nameBuffer)) {
            nameBuffer[nameLength] = '\0';
            status = state->getter(nameBuffer, value);
        } else {
            std::vector<char> nameVector(nameLength + 1);
            nameLength = JS_EncodeStringToBuffer(cx, name, &nameVector[0], nameVector.size() - 1);
            nameVector[nameLength] = '\0';
            status = state->getter(&nameVector[0], value);
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
    auto state = Object::GetState(obj);
    if (state != nullptr && state->setter != nullptr) {
        Value value(cx, vp);
        
        char nameBuffer[256];
        auto name = JSID_TO_STRING(id);                
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        
        if (nameLength < sizeof(nameBuffer)) {
            nameBuffer[nameLength] = '\0';    
            return state->setter(nameBuffer, value);    
        } else {
            std::vector<char> nameVector(nameLength + 1);
            nameLength = JS_EncodeStringToBuffer(cx, name, &nameVector[0], nameVector.size() - 1);
            nameVector[nameLength] = '\0';
            return state->setter(&nameVector[0], value);
        }    
    } else {
        // TODO: what will this do to the JS?
        vp.setUndefined();
        return true;
    }
}

bool rs::jsapi::Object::CallFunction(JSContext* cx, unsigned argc, JS::Value* vp) {
    char nameBuffer[256];
    const char* name = nameBuffer;
    
    auto args = JS::CallArgsFromVp(argc, vp);
    auto func = JS_ValueToFunction(cx, args.calleev());
    if (func != nullptr) {
        auto funcName = JS_GetFunctionId(func);                
        if (funcName != nullptr) {
            auto nameLength = JS_EncodeStringToBuffer(cx, funcName, nameBuffer, sizeof(nameBuffer));
            if ((nameLength + 1) < sizeof(nameBuffer)) {
                nameBuffer[nameLength] = '\0';
            } else {
                std::vector<char> vBuffer(nameLength + 1);
                JS_EncodeStringToBuffer(cx, funcName, &vBuffer[0], nameLength);
                vBuffer[nameLength] = '\0';
                name = &vBuffer[0];
            }
        }
    }
    
    if (name == nullptr) {
        // TODO: test this case
        JS_ReportError(cx, "Unable to find function in libjsapi object");
        return false;        
    } else {
        auto state = Object::GetState(args.thisv().toObjectOrNull());
        if (state == nullptr) {
            // TODO: test this case
            JS_ReportError(cx, "Unable to find function callback in libjsapi object");
            return false;
        } else {
            try {
                std::vector<Value> vArgs;
                for (int i = 0; i < argc; ++i) {
                    vArgs.emplace_back(cx, args.get(i));
                }

                Value result(cx);
                state->functions[name](vArgs, result);
                args.rval().set(result);
                return true;
            } catch (const std::exception& ex) {
                JS_ReportError(cx, ex.what());
                return false;
            }
        }        
    }
}

void rs::jsapi::Object::Finalize(JSFreeOp* fop, JSObject* obj) {
    auto state = Object::GetState(obj);
    if (state != nullptr && state->finalizer != nullptr) {
        state->finalizer();
    }
    
    SetState(obj, nullptr);
    delete state;    
}

rs::jsapi::Object::ObjectState* rs::jsapi::Object::GetState(JSObject* obj) {
    auto state = JS_GetPrivate(obj);
    return reinterpret_cast<ObjectState*>(state);
}

void rs::jsapi::Object::SetState(JSObject* obj, ObjectState* state) {
    JS_SetPrivate(obj, state);    
}

bool rs::jsapi::Object::SetPrivate(Value& value, uint64_t data, void* ptr) {
    auto set = false;
    if (value.isObject()) {
        auto obj = value.toObject();
        auto klass = JS_GetClass(obj);
        if (klass != nullptr && std::strcmp(klass->name, Object::class_.name) == 0) {
            auto state = GetState(obj);
            state->data = data;
            state->ptr = ptr;
            set = true;
        }
    }
    
    return set;
}

bool rs::jsapi::Object::GetPrivate(const Value& value, uint64_t& data, void*& ptr) {
    auto get = false;
    if (value.isObject()) {
        auto obj = value.toObject();
        auto klass = JS_GetClass(obj);
        if (klass != nullptr && std::strcmp(klass->name, Object::class_.name) == 0) {
            auto state = GetState(obj);
            data = state->data;
            ptr = state->ptr;
            get = true;
        }
    }
    
    return get;
}