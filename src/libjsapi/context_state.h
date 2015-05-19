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

#ifndef RS_JSAPI_CONTEXT_STATE_H
#define RS_JSAPI_CONTEXT_STATE_H

#include <atomic>

#include <jsapi.h>

namespace rs {
namespace jsapi {

class ContextState final {
public:
    
    class AutoRequest final {
    public:
        AutoRequest(JSContext* cx) : cx_(cx) {
            ContextState::BeginRequest(cx);
        }
        
        ~AutoRequest() {
            ContextState::EndRequest(cx_);
        }
    private:
        JSContext* cx_;
    };
    
    static bool NewState(JSContext* cx, void* ptr = nullptr);
    static bool DeleteState(JSContext* cx);
    
    static bool BeginRequest(JSContext* cx);
    static bool EndRequest(JSContext* cx);
    
    static bool FlushRequests(JSContext* cx);
    
    static bool SetDataPtr(JSContext* cx, void* ptr);
    static void* GetDataPtr(JSContext* cx);
    
private:

    struct State final {
    public:
        State() : requestCount_(0), ptr_(nullptr) {}
        
        std::atomic<long> requestCount_;
        void* ptr_;
    };
    
    static State* GetState(JSContext* cx);
    
};

}}

#endif	/* RS_JSAPI_CONTEXT_STATE_H */
