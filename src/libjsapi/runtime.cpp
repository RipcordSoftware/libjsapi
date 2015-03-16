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

std::atomic<int> rs::jsapi::Runtime::Instance::count_;
std::mutex rs::jsapi::Runtime::Instance::m_;

rs::jsapi::Runtime::Instance::Instance() {
    if (count_ == 0) {
        std::lock_guard<std::mutex> lk(m_);
        if (count_ == 0) {
            JS_Init();
            count_ += 1;
        }
    }
    
}

rs::jsapi::Runtime::Instance::~Instance() {
    count_ -= 1;
    
    if (count_ == 0) {
        JS_ShutDown();
    }
}

rs::jsapi::Runtime::Runtime() :
    rt_(JS_NewRuntime(8L * 1024L * 1024L, JSUseHelperThreads::JS_USE_HELPER_THREADS)),
    cx_(JS_NewContext(rt_, 8192)),
    global_(cx_, JS_NewGlobalObject(cx_, &globalClass, nullptr, JS::DontFireOnNewGlobalHook)),
    ac_(cx_, global_) {
    JS_InitStandardClasses(cx_, global_);
    JS_BeginRequest(cx_);
}

rs::jsapi::Runtime::~Runtime() {
    JS_EndRequest(cx_);
    JS_DestroyContext(cx_);
    JS_DestroyRuntime(rt_);
}