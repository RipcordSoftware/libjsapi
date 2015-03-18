#include "runtime.h"
#include "exceptions.h"

std::atomic<int> rs::jsapi::Runtime::Instance::count_;
std::mutex rs::jsapi::Runtime::Instance::m_;

rs::jsapi::Runtime::Instance::Instance() {
    if (count_ == 0) {
        std::lock_guard<std::mutex> lk(m_);
        if (count_ == 0) {
            JS_Init();
            count_ += 1;
        }
    } else {
        count_ += 1;
    }   
}

rs::jsapi::Runtime::Instance::~Instance() {
    count_ -= 1;
    
    if (count_ == 0) {
        JS_ShutDown();
    }
}

rs::jsapi::Runtime::Runtime() :
    threadId_(std::this_thread::get_id()),
    rt_(JS_NewRuntime(8L * 1024L * 1024L, JSUseHelperThreads::JS_USE_HELPER_THREADS)),
    cx_(*this) {
}

rs::jsapi::Runtime::~Runtime() {
    cx_.DestroyContext();
    JS_DestroyRuntime(rt_);
}

JSRuntime* rs::jsapi::Runtime::getRuntime() {
    if (threadId_ != std::this_thread::get_id()) {
        throw RuntimeWrongThreadException();
    }
    
    return rt_; 
}

rs::jsapi::Context& rs::jsapi::Runtime::getContext() {
    if (threadId_ != std::this_thread::get_id()) {
        throw RuntimeWrongThreadException();
    }
    
    return cx_; 
}

std::unique_ptr<rs::jsapi::Context> rs::jsapi::Runtime::NewContext() {
    if (threadId_ != std::this_thread::get_id()) {
        throw RuntimeWrongThreadException();
    }
    
    return std::unique_ptr<Context>(new Context(*this));
}