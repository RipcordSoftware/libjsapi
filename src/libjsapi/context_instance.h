/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Ripcord Software Ltd
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

#ifndef RS_JSAPI_CONTEXT_INSTANCE_H
#define RS_JSAPI_CONTEXT_INSTANCE_H

#include <atomic>
#include <mutex>

class JSContext;

namespace rs {
namespace jsapi {

class ContextInstance {
public:
    ContextInstance();
    ~ContextInstance();
    
    static JSContext* GetParentContext() { return context_; }

private:
    static std::atomic<bool> initCalled_;
    static std::atomic<int> count_;
    static std::mutex m_;
    static std::atomic<JSContext*> context_;
    
    static void AtExit();
};

}}

#endif	/* RS_JSAPI_CONTEXT_INSTANCE_H */

