#include "context.h"
#include "runtime.h"
#include "value.h"
#include "result.h"
#include "function_arguments.h"

// The class of the global object.
static JSClass globalClass = {
    "global",
    JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub,
    JS_DeletePropertyStub,
    JS_PropertyStub,
    JS_StrictPropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    nullptr, nullptr, nullptr, nullptr,
    JS_GlobalObjectTraceHook
};

rs::jsapi::Context::Context(Runtime& rt) : 
        cx_(JS_NewContext(rt.getRuntime(), 8192)),
        global_(cx_, JS_NewGlobalObject(cx_, &globalClass, nullptr, JS::DontFireOnNewGlobalHook)),
        oldCompartment_(nullptr) {
    oldCompartment_ = JS_EnterCompartment(cx_, global_);
    JS_InitStandardClasses(cx_, global_);
    
    JS_SetContextPrivate(cx_, this);
    JS_SetErrorReporter(cx_, &ReportError);
}

rs::jsapi::Context::~Context() {
    DestroyContext();
}

void rs::jsapi::Context::ReportError(JSContext* cx, const char* message, JSErrorReport* report) {
    auto that = static_cast<Context*>(JS_GetContextPrivate(cx));
    if (that != nullptr) {
        that->exception_.reset(new ScriptException(message, report));
    }
}

std::unique_ptr<rs::jsapi::ScriptException> rs::jsapi::Context::getError() {
    return std::move(exception_);
}

void rs::jsapi::Context::DestroyContext() {
    if (cx_) {
        JS_SetErrorReporter(cx_, nullptr);
        JS_SetContextPrivate(cx_, nullptr);
        
        JS_LeaveCompartment(cx_, oldCompartment_);
        oldCompartment_ = nullptr;
        
        JS_DestroyContext(cx_);
        cx_ = nullptr;
    }
}

bool rs::jsapi::Context::Evaluate(const char* script) {
    Result result(*this);        
    return Evaluate(script, result);
}

bool rs::jsapi::Context::Evaluate(const char* script, Result& result) {
    JS::CompileOptions options(cx_);
    auto status = JS::Evaluate(cx_, global_, options, script, ::strlen(script), result);
    
    auto error = getError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}

bool rs::jsapi::Context::Call(const char* name) {
    Result result(*this);        
    return Call(name, result);
}

bool rs::jsapi::Context::Call(const char* name, Result& result) {
    auto status = JS_CallFunctionName(cx_, global_, name, JS::HandleValueArray::empty(), result);    
    
    auto error = getError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}

bool rs::jsapi::Context::Call(const char* name, const FunctionArguments& args) {
    Result result(*this);        
    return Call(name, args, result);
}

bool rs::jsapi::Context::Call(const char* name, const FunctionArguments& args, Result& result) {
    auto status = JS_CallFunctionName(cx_, global_, name, args, result);    
    
    auto error = getError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}