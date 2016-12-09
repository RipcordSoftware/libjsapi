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

#include "context.h"

#include <cstring>
#include <cstdlib>

#include <js/Initialization.h>

#include "value.h"
#include "function_arguments.h"

// The class of the global object.
static JSClass globalClass = {
    "global",
    JSCLASS_GLOBAL_FLAGS
};

JS::CompartmentOptions rs::jsapi::Context::options_;

rs::jsapi::Context::Context(uint32_t maxBytes, uint32_t maxNurseryBytes,
        bool enableBaselineCompiler, bool enableIonCompiler) : 
        cx_(NewContext(maxBytes, maxNurseryBytes)),
        global_(cx_, JS_NewGlobalObject(cx_, &globalClass, nullptr, JS::DontFireOnNewGlobalHook, options_)),
        oldCompartment_(nullptr) {
    JS::SetWarningReporter(cx_, &ReportWarning);

    ContextState::NewState(cx_, ReportWarning, this);
    
    oldCompartment_ = JS_EnterCompartment(cx_, global_);
    JS_InitStandardClasses(cx_, global_);

    if (enableBaselineCompiler) {
        JS_SetGlobalJitCompilerOption(cx_, JSJITCOMPILER_BASELINE_ENABLE, 1);
        
        if (enableIonCompiler) {
            JS_SetGlobalJitCompilerOption(cx_, JSJITCOMPILER_ION_ENABLE, 1);
        }
    } 
}

rs::jsapi::Context::~Context() {
    DestroyContext();
}

void rs::jsapi::Context::ReportWarning(JSContext* cx, const char* message, JSErrorReport* report) {
    auto that = static_cast<Context*>(ContextState::GetDataPtr(cx));
    if (that != nullptr) {
        that->exception_.reset(new ScriptException(message, report));
    }
}

std::unique_ptr<rs::jsapi::ScriptException> rs::jsapi::Context::GetError() {
    std::unique_ptr<ScriptException> error;
    if (JS_IsExceptionPending(cx_)) {
        error = GetContextException();
    } else {
        error = GetContextError();
    }

    return error;
}

std::unique_ptr<rs::jsapi::ScriptException> rs::jsapi::Context::GetContextError() {
    return std::move(exception_);
}

std::unique_ptr<rs::jsapi::ScriptException> rs::jsapi::Context::GetContextException() {
    std::unique_ptr<rs::jsapi::ScriptException> error;
    Value ex(cx_);
        
    if (JS_GetPendingException(cx_, ex)) {
        JS_ClearPendingException(cx_);

        auto report = JS_ErrorFromException(cx_, ex);
        error.reset(new ScriptException(report));
    }
    
    return error;
}

void rs::jsapi::Context::DestroyContext() {
    if (cx_) {
        ContextState::DetachWarningReporter(cx_);
        
        global_.reset();
        
        JS_LeaveCompartment(cx_, oldCompartment_);
        oldCompartment_ = nullptr;
        
        ContextState::DeleteState(cx_);
        
        JS_DestroyContext(cx_);
        cx_ = nullptr;
    }
}

bool rs::jsapi::Context::Evaluate(const char* script) {
    CheckCallingThread();
    
    Value result(*this);        
    return Evaluate(script, result);
}

bool rs::jsapi::Context::Evaluate(const char* script, Value& result) {
    CheckCallingThread();
    
    JSAutoRequest ar(cx_);    
    JS::CompileOptions options(cx_);
    auto status = JS::Evaluate(cx_, options, script, std::strlen(script), result);
    
    auto error = GetError();
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
    CheckCallingThread();
    
    JSAutoRequest ar(cx_);    
    auto status = JS_CallFunctionName(cx_, global_, name, JS::HandleValueArray::empty(), result);    
    
    auto error = GetError();
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
    CheckCallingThread();
    
    JSAutoRequest ar(cx_);
    auto status = JS_CallFunctionName(cx_, global_, name, args, result);    
    
    auto error = GetError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}

bool rs::jsapi::Context::Call(Value& value, const FunctionArguments& args, bool throwOnError) {    
    auto cx = value.getContext();
    rs::jsapi::Value result{cx};
    return Context::Call(value, args, result, throwOnError);
}

bool rs::jsapi::Context::Call(Value& value, const FunctionArguments& args, Value& result, bool throwOnError) {
    auto status = false;
    
    auto cx = value.getContext();
    auto that = static_cast<Context*>(ContextState::GetDataPtr(cx));
    if (that) {
        that->CheckCallingThread();
        
        JSAutoRequest ar{cx};
        status = JS_CallFunctionValue(cx, that->global_, value, args, result);
        
        auto error = that->GetError();
        if (throwOnError && !!error) {
            throw *error;
        }
    }
    
    return status;
}

JSContext* rs::jsapi::Context::NewContext(uint32_t maxBytes, uint32_t maxNurseryBytes) {
    auto cx = JS_NewContext(maxBytes, maxNurseryBytes, ContextInstance::GetParentContext());
    JS::InitSelfHostedCode(cx);
    return cx;
}
