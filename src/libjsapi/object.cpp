/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Ripcord Software Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/

#include "object.h"
#include "context.h"
#include "value.h"
#include "vector_utils.h"

#include <cstring>

JSClass rs::jsapi::Object::class_ = { 
    "rs_jsapi_object", JSCLASS_HAS_PRIVATE, nullptr, nullptr,
    Object::Get, Object::Set, nullptr, nullptr, 
    nullptr, Object::Finalize 
};

bool rs::jsapi::Object::Create(JSContext* cx, const std::vector<const char*>& properties,
        const GetCallback& getter, const SetCallback& setter, const std::vector<std::pair<const char*, FunctionCallback>>& functions,
        const FinalizeCallback& finalizer, Value& obj) {
    JSAutoRequest ar(cx);
    JS::RootedObject newObj(cx, JS_NewObject(cx, &class_, JS::NullPtr()));
    
    if (newObj) {
        auto state = new ObjectState { getter, setter, finalizer, Functions(), 0, nullptr };
        
        for (auto p : properties) {
            JS_DefineProperty(cx, newObj, p, JS::NullHandleValue, JSPROP_ENUMERATE);
        }

        for (auto f : functions) {
            JS_DefineFunction(cx, newObj, f.first, Object::CallFunction, 0, JSPROP_ENUMERATE | JSFUN_STUB_GSOPS);
            state->functions.emplace(f.first, f.second);
        }
        
        Object::SetState(newObj, state);

        obj.set(newObj);
    }
    
    return newObj;
}

bool rs::jsapi::Object::IsObject(const Value& value) {
    auto isObject = value.isObject() && !value.isNull();
    if (isObject) {
        auto obj = value.toObject();
        isObject = obj;
        if (isObject) {
            auto klass = JS_GetClass(obj);
            isObject = klass && klass->name && std::strcmp(klass->name, class_.name) == 0;
        }
    }
    return isObject; 
}

bool rs::jsapi::Object::Get(JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp) {
    JSAutoRequest ar(cx);
    auto state = Object::GetState(cx, obj);    
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

bool rs::jsapi::Object::Set(JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool strict, JS::MutableHandleValue vp) {
    JSAutoRequest ar(cx);
    auto state = Object::GetState(cx, obj);
    if (state != nullptr && state->setter != nullptr) {
        Value value(cx, vp);
        
        char nameBuffer[256];
        auto name = JSID_TO_STRING(id);                
        auto nameLength = JS_EncodeStringToBuffer(cx, name, nameBuffer, sizeof(nameBuffer) - 1);
        
        try {
            if (nameLength < sizeof(nameBuffer)) {
                nameBuffer[nameLength] = '\0';    
                state->setter(nameBuffer, value);    
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

bool rs::jsapi::Object::CallFunction(JSContext* cx, unsigned argc, JS::Value* vp) {
    JSAutoRequest ar(cx);
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
        auto that = args.thisv();
        auto state = that.isObjectOrNull() ? Object::GetState(cx, JS::RootedObject(cx, that.toObjectOrNull())) : nullptr;
        if (state == nullptr) {
            // TODO: test this case
            JS_ReportError(cx, "Unable to find function callback in libjsapi object");
            return false;
        } else {
            try {
#if __clang_major__ < 4
                std::vector<Value> vArgs;
#else
                static thread_local std::vector<Value> vArgs;
#endif

                VectorUtils::ScopedVectorCleaner<Value> clean(vArgs);
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

rs::jsapi::Object::ObjectState* rs::jsapi::Object::GetState(JSContext* cx, JS::HandleObject obj) {
    JSAutoRequest ar(cx);
    auto state = JS_GetInstancePrivate(cx, obj, &Object::class_, nullptr);
    return reinterpret_cast<ObjectState*>(state);
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