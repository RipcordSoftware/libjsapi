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

#include <js/Initialization.h>

#include "runtime.h"

#include <algorithm>
#include <cstdlib>

#include "exceptions.h"
#include "context_state.h"

#ifdef __GNUC__
#define INIT_PRIORITY(N) __attribute__ ((init_priority (N + 101)))
#else
#error "Unable to set static initiaiization order on this platform. If you wish to fix this for your compiler please PR the change."
#endif

std::atomic<bool> rs::jsapi::Runtime::Instance::initCalled_ INIT_PRIORITY(0);
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
    if (count_ == 0 && !initCalled_) {
        std::lock_guard<std::mutex> lk(m_);
        if (count_ == 0 && !initCalled_) {
            JS_Init();
            
            // create the first runtime and context and then destroy them, we
            // need to do it here to ensure they are done on the same thread
            auto rt = JS_NewRuntime(8 * 1024 * 1024);
            auto cx = JS_NewContext(rt, 32 * 1024);
            JS_DestroyContext(cx);
            JS_DestroyRuntime(rt);
            
            // shutdown JS at process end
            std::atexit([] { JS_ShutDown(); });

            // keep track of the number of instances
            ++count_;

            // don't allow init to be called more than once
            initCalled_= true;
        } else {
            ++count_;
        }
    } else {
        ++count_;
    }   
}

rs::jsapi::Runtime::Instance::~Instance() {    
    --count_;
}

rs::jsapi::Runtime::Runtime(uint32_t maxBytes, bool enableBaselineCompiler, bool enableIonCompiler) :
        threadId_(std::this_thread::get_id()),
        threadGuard_(threadId_),
        rt_(JS_NewRuntime(maxBytes)),
        cx_(*this) {
    if (enableBaselineCompiler) {
        JS_SetGlobalJitCompilerOption(rt_, JSJITCOMPILER_BASELINE_ENABLE, 1);
        
        if (enableIonCompiler) {
            JS_SetGlobalJitCompilerOption(rt_, JSJITCOMPILER_ION_ENABLE, 1);
        }
    }
    
    JS_SetErrorReporter(rt_, &ReportError);
}

rs::jsapi::Runtime::~Runtime() {
    JS_SetErrorReporter(rt_, nullptr);
    
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

void rs::jsapi::Runtime::ReportError(JSContext *cx, const char *message, JSErrorReport *report) {
    ContextState::ReportError(cx, message, report);
}