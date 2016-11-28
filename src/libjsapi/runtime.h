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

#ifndef RS_JSAPI_RUNTIME_H
#define RS_JSAPI_RUNTIME_H

#include <jsapi.h>

#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <vector>

#include "exceptions.h"
#include "context.h"

namespace rs {
namespace jsapi {
    
class Value;    
class FunctionArguments;

class Runtime final {
public:
    Runtime(uint32_t maxBytes = 64 * 1024 * 1024, bool enableBaselineCompiler = true, bool enableIonCompiler = false);
    Runtime(const Runtime&) = delete;
    ~Runtime();
    
    bool Evaluate(const char* script);
    bool Evaluate(const char* script, Value& result);
    bool Call(const char* name);
    bool Call(const char* name, const FunctionArguments& args);
    bool Call(const char* name, Value& result);
    bool Call(const char* name, const FunctionArguments& args, Value& result);
    
    Runtime& operator=(const Runtime&) = delete;
    
    std::unique_ptr<Context> NewContext();
    
    JSRuntime* getRuntime();
    Context& getContext() { return cx_; }
    
    operator Context&() { 
        if (threadId_ != std::this_thread::get_id()) {
            throw RuntimeWrongThreadException();
        }
        
        return cx_; 
    }
    
    operator JSContext*() { 
        if (threadId_ != std::this_thread::get_id()) {
            throw RuntimeWrongThreadException();
        }
        
        return cx_; 
    }
    
    void GCNow() { JS_GC(rt_); }
    
    void MaybeGC() { JS_MaybeGC(cx_); }
    
private:
    
    class Instance {
    public:
        Instance();
        ~Instance();
        
    private:
        static std::atomic<bool> initCalled_;
        static std::atomic<int> count_;
        static std::mutex m_;
    };  
    
    class RuntimeThreadGuard {
    public:
        RuntimeThreadGuard(std::thread::id id);
    };
    
    static std::vector<std::thread::id> activeRuntimes_;
    static std::mutex activeRuntimesLock_;
    
    const std::thread::id threadId_;
    const RuntimeThreadGuard threadGuard_;    
    Instance inst_;
    JSRuntime* rt_;
    Context cx_;
    
    static bool AddRuntime(std::thread::id id);
    static bool RemoveRuntime(std::thread::id id);
    
    static void ReportError(JSContext *cx, const char *message, JSErrorReport *report);
};

}}

#endif	/* RS_JSAPI_RUNTIME_H */

