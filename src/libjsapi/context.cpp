#include "context.h"
#include "runtime.h"

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
    auto rt = static_cast<Context*>(JS_GetContextPrivate(cx));
    if (rt != nullptr) {
        rt->error_ = message;
    }
}

bool rs::jsapi::Context::HasError() {
    return error_.length() > 0;
}

const std::string& rs::jsapi::Context::getError() {
    return error_;
}

bool rs::jsapi::Context::ClearError() {
    error_.clear();
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