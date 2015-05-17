#include "global.h"
#include "context.h"
#include "value.h"

JSClass rs::jsapi::Global::privateFunctionStateClass_ = { 
    "rs_jsapi_global_function_object", JSCLASS_HAS_PRIVATE, JS_PropertyStub, JS_DeletePropertyStub,
    JS_PropertyStub, JS_StrictPropertyStub, JS_EnumerateStub, JS_ResolveStub, 
    JS_ConvertStub, Global::Finalize 
};

const char* rs::jsapi::Global::privateFunctionStatePropertyName_ = "__rs_jsapi__private_global_function_state";

bool rs::jsapi::Global::DefineProperty(Context& cx, const char* name, const Value& value, unsigned attrs) {
    if (value.isObject()) {
        return JS_DefineProperty(cx, cx.getGlobal(), name, value.getHandleObject(), attrs, nullptr, nullptr);
    } else {    
        return JS_DefineProperty(cx, cx.getGlobal(), name, value.getHandleValue(), attrs, nullptr, nullptr);
    }
}

bool rs::jsapi::Global::DefineProperty(Context& cx, const char* name, JSNative getter, JSNative setter, unsigned attrs) {
    return JS_DefineProperty(cx, cx.getGlobal(), name, JS::NullHandleValue, 
        attrs | JSPROP_NATIVE_ACCESSORS,
        reinterpret_cast<JSPropertyOp>(getter), reinterpret_cast<JSStrictPropertyOp>(setter));
}

bool rs::jsapi::Global::DefineFunction(Context& cx, const char* name, FunctionCallback callback, unsigned attrs) {
    JS::RootedFunction func(cx, JS_NewFunction(cx, CallFunction, 0, 0, JS::NullPtr(), name));
    JS::RootedObject funcObj(cx, JS_GetFunctionObject(func));
    
    JS::RootedObject privateFuncObj(cx, JS_NewObject(cx, &privateFunctionStateClass_, JS::NullPtr(), JS::NullPtr()));
    JS_SetPrivate(privateFuncObj, new GlobalFunctionState(callback));
    JS_DefineProperty(cx, funcObj, privateFunctionStatePropertyName_, privateFuncObj, 0, nullptr, nullptr);

    return JS_DefineProperty(cx, cx.getGlobal(), name, funcObj, attrs, nullptr, nullptr);
}

bool rs::jsapi::Global::CallFunction(JSContext* cx, unsigned argc, JS::Value* vp) {
    auto args = JS::CallArgsFromVp(argc, vp);
    
    auto state = Global::GetFunctionState(&args.callee(), cx, privateFunctionStatePropertyName_);
    if (state) {
        try {
            std::vector<Value> vArgs;
            for (int i = 0; i < argc; ++i) {
                vArgs.emplace_back(cx, args.get(i));
            }

            Value result(cx);
            state->function_(vArgs, result);
            args.rval().set(result);
            return true;
        } catch (const std::exception& ex) {
            JS_ReportError(cx, ex.what());
            return false;
        }
    } else {
        JS_ReportError(cx, "Unable to find function callback in libjsapi object");
        return false;
    }
}

void rs::jsapi::Global::Finalize(JSFreeOp* fop, JSObject* obj) {
    auto data = GetFunctionState(obj);
    if (data) {
        delete data;
        SetFunctionState(obj, nullptr);
    }
}

rs::jsapi::Global::GlobalFunctionState* rs::jsapi::Global::GetFunctionState(JSObject* obj, JSContext* cx, const char* propName) {
    GlobalFunctionState* state = nullptr;
    
    if (obj) {
        rs::jsapi::Value result(cx);
        if (JS_GetProperty(cx, JS::RootedObject(cx, obj), privateFunctionStatePropertyName_, result) && result.isObject()) {
            state = GetFunctionState(result.toObject());
        }
    }

    return state;
}

rs::jsapi::Global::GlobalFunctionState* rs::jsapi::Global::GetFunctionState(JSObject* obj) {
    auto state = obj != nullptr ? JS_GetPrivate(obj) : nullptr;
    return reinterpret_cast<GlobalFunctionState*>(state);
}

void rs::jsapi::Global::SetFunctionState(JSObject* obj, GlobalFunctionState* state) {
    JS_SetPrivate(obj, state);
}

bool rs::jsapi::Global::DeleteProperty(Context& cx, const char* name) {
    return JS_DeleteProperty(cx, cx.getGlobal(), name);
}

bool rs::jsapi::Global::DeleteFunction(Context& cx, const char* name) {
    return JS_DeleteProperty(cx, cx.getGlobal(), name);
}