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

#include "runtime.h"

#include <algorithm>

#include "exceptions.h"

#ifdef __GNUC__
#define INIT_PRIORITY(N) __attribute__ ((init_priority (N + 101)))
#else
#error "Unable to set static initiaiization order on this platform. If you wish to fix this for your compiler please PR the change."
#endif

std::atomic<int> rs::jsapi::Runtime::Instance::count_ INIT_PRIORITY(0);
std::mutex rs::jsapi::Runtime::Instance::m_ INIT_PRIORITY(0);

std::vector<std::thread::id> rs::jsapi::Runtime::activeRuntimes_ INIT_PRIORITY(0);
std::mutex rs::jsapi::Runtime::activeRuntimesLock_ INIT_PRIORITY(0);

rs::jsapi::Runtime::RuntimeThreadGuard::RuntimeThreadGuard(std::thread::id id) {
    if (!AddRuntime(id)) {
        throw RuntimeThreadInstanceException();
    }
}

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

rs::jsapi::Runtime::Runtime(uint32_t maxBytes, bool enableBaselineCompiler, bool enableIonCompiler) :
        threadId_(std::this_thread::get_id()),
        threadGuard_(threadId_),
        rt_(JS_NewRuntime(maxBytes, JSUseHelperThreads::JS_USE_HELPER_THREADS)),
        cx_(*this) {
    if (enableBaselineCompiler) {
        JS_SetGlobalJitCompilerOption(rt_, JSJITCOMPILER_BASELINE_ENABLE, 1);
        
        if (enableIonCompiler) {
            JS_SetGlobalJitCompilerOption(rt_, JSJITCOMPILER_ION_ENABLE, 1);
        }
    }
}

rs::jsapi::Runtime::~Runtime() {
    cx_.DestroyContext();
    JS_DestroyRuntime(rt_);
    
    RemoveRuntime(threadId_);
}

JSRuntime* rs::jsapi::Runtime::getRuntime() {
    if (threadId_ != std::this_thread::get_id()) {
        throw RuntimeWrongThreadException();
    }
    
    return rt_; 
}

std::unique_ptr<rs::jsapi::Context> rs::jsapi::Runtime::NewContext() {
    if (threadId_ != std::this_thread::get_id()) {
        throw RuntimeWrongThreadException();
    }
    
    return std::unique_ptr<Context>(new Context(*this));
}

bool rs::jsapi::Runtime::AddRuntime(std::thread::id id) {
    std::lock_guard<std::mutex> lock(activeRuntimesLock_);
    
    auto iter = std::find(activeRuntimes_.cbegin(), activeRuntimes_.cend(), id);
    bool add = iter == activeRuntimes_.cend();
    if (add) {
        activeRuntimes_.push_back(id);
    }
    
    return add;
}

bool rs::jsapi::Runtime::RemoveRuntime(std::thread::id id) {
    std::lock_guard<std::mutex> lock(activeRuntimesLock_);   
    
    auto iter = std::find(activeRuntimes_.begin(), activeRuntimes_.end(), id);
    bool remove = iter != activeRuntimes_.end();
    if (remove) {
        activeRuntimes_.erase(iter);
    }
    
    return remove;
}

bool rs::jsapi::Runtime::Call(const char* name) {
    return cx_.Call(name);
}

bool rs::jsapi::Runtime::Call(const char* name, const FunctionArguments& args) {
    return cx_.Call(name, args);
}

bool rs::jsapi::Runtime::Call(const char* name, Value& result) {
    return cx_.Call(name, result);
}

bool rs::jsapi::Runtime::Call(const char* name, const FunctionArguments& args, Value& result) {
    return cx_.Call(name, args, result);
}

bool rs::jsapi::Runtime::Evaluate(const char* script) {
    return cx_.Evaluate(script);
}

bool rs::jsapi::Runtime::Evaluate(const char* script, Value& result) {
    return cx_.Evaluate(script, result);
}