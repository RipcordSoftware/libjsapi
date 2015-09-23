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

#ifndef RS_JSAPI_CONTEXT_H
#define RS_JSAPI_CONTEXT_H

#include <string>
#include <memory>

#include <jsapi.h>

#include "exceptions.h"
#include "script.h"

namespace rs {
namespace jsapi {

class Runtime;
class FunctionArguments;
class Value;

class Context final {
    friend class Runtime;
public:
    Context(Runtime& rt);    
    ~Context();
    
    bool Evaluate(const char* script);
    bool Evaluate(const char* script, Value& result);
    bool Call(const char* name);
    bool Call(const char* name, const FunctionArguments& args);
    bool Call(const char* name, Value& result);
    bool Call(const char* name, const FunctionArguments& args, Value& result);
    
    Context(const Context&) = delete;
    Context& operator =(const Context&) = delete;
    
    JSContext* getContext() { return cx_; }
    JS::HandleObject getGlobal() { return global_; }
    
    operator JSContext*() { return cx_; }
    
    void MaybeGC() { JS_MaybeGC(cx_); }
        
private:
    friend bool Script::Compile();
    friend bool Script::Execute();
    friend bool Script::Execute(Value&);
    
    std::unique_ptr<ScriptException> getError();
    
    static void ReportError(JSContext *cx, const char *message, JSErrorReport *report);
    
    void DestroyContext();
   
    JSContext* cx_;
    JS::PersistentRootedObject global_;
    JSCompartment* oldCompartment_;
    
    std::unique_ptr<ScriptException> exception_;
};

}}

#endif	/* RS_JSAPI_CONTEXT_H */