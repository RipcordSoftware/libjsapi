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

#include "context_state.h"

bool rs::jsapi::ContextState::NewState(JSContext* cx, JS::WarningReporter warningReporter, void* ptr) {
    auto state = GetState(cx);
    if (state == nullptr) {
        auto state = new State(warningReporter, ptr);
        JS_SetContextPrivate(cx, state);
        return true;
    } else {
        return false;
    }
}

bool rs::jsapi::ContextState::DeleteState(JSContext* cx) {
    auto state = GetState(cx);
    if (state != nullptr) {
        JS_SetContextPrivate(cx, nullptr);
        delete state;
        return true;
    } else {
        return false;
    }
}

rs::jsapi::ContextState::State* rs::jsapi::ContextState::GetState(JSContext* cx) {
    auto state = reinterpret_cast<State*>(JS_GetContextPrivate(cx));
    return state;
}

bool rs::jsapi::ContextState::SetDataPtr(JSContext* cx, void* ptr) {
    auto state = GetState(cx);
    if (state != nullptr) {
        state->ptr_ = ptr;
        return true;
    } else {
        return false;
    }
}

void* rs::jsapi::ContextState::GetDataPtr(JSContext* cx) {
    void* ptr = nullptr;
    auto state = GetState(cx);
    if (state != nullptr) {
        ptr = state->ptr_;
    }
    return ptr;
}

void rs::jsapi::ContextState::ReportWarning(JSContext* cx, const char* message, JSErrorReport* report) {
    auto state = GetState(cx);
    if (state != nullptr) {
        auto reporter = state->warningReporter_.load();
        if (reporter != nullptr) {
            reporter(cx, message, report);
        }
    }
}

void rs::jsapi::ContextState::DetachWarningReporter(JSContext* cx) {
    auto state = GetState(cx);
    if (state != nullptr) {
        state->warningReporter_ = nullptr;
    }
}