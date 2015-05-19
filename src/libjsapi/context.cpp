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

#include "context.h"
#include "runtime.h"
#include "value.h"
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
        cx_(JS_NewContext(rt.getRuntime(), 32 * 1024)),
        global_(cx_, JS_NewGlobalObject(cx_, &globalClass, nullptr, JS::DontFireOnNewGlobalHook)),
        oldCompartment_(nullptr) {
    ContextState::NewState(cx_, this);
    
    oldCompartment_ = JS_EnterCompartment(cx_, global_);
    JS_InitStandardClasses(cx_, global_);
    
    JS_SetErrorReporter(cx_, &ReportError);
}

rs::jsapi::Context::~Context() {
    DestroyContext();
}

void rs::jsapi::Context::ReportError(JSContext* cx, const char* message, JSErrorReport* report) {
    auto that = static_cast<Context*>(ContextState::GetDataPtr(cx));
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
        
        JS_LeaveCompartment(cx_, oldCompartment_);
        oldCompartment_ = nullptr;
        
        ContextState::FlushRequests(cx_);
        ContextState::DeleteState(cx_);
        
        JS_DestroyContext(cx_);
        cx_ = nullptr;
    }
}

bool rs::jsapi::Context::Evaluate(const char* script) {
    Value result(*this);        
    return Evaluate(script, result);
}

bool rs::jsapi::Context::Evaluate(const char* script, Value& result) {
    JSAutoRequest ar(cx_);    
    JS::CompileOptions options(cx_);
    auto status = JS::Evaluate(cx_, global_, options, script, ::strlen(script), result);
    
    auto error = getError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}

bool rs::jsapi::Context::Call(const char* name) {
    Value result(*this);        
    return Call(name, result);
}

bool rs::jsapi::Context::Call(const char* name, Value& result) {
    JSAutoRequest ar(cx_);    
    auto status = JS_CallFunctionName(cx_, global_, name, JS::HandleValueArray::empty(), result);    
    
    auto error = getError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}

bool rs::jsapi::Context::Call(const char* name, const FunctionArguments& args) {
    Value result(*this);        
    return Call(name, args, result);
}

bool rs::jsapi::Context::Call(const char* name, const FunctionArguments& args, Value& result) {
    JSAutoRequest ar(cx_);    
    auto status = JS_CallFunctionName(cx_, global_, name, args, result);    
    
    auto error = getError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}