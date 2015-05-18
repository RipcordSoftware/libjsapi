#include "context_state.h"

bool rs::jsapi::ContextState::NewState(JSContext* cx, void* ptr) {
    auto state = GetState(cx);
    if (state == nullptr) {
        auto state = new State();
        state->ptr_ = ptr;
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

bool rs::jsapi::ContextState::BeginRequest(JSContext* cx) {
    auto state = GetState(cx);
    if (state != nullptr) {
        JS_BeginRequest(cx);
        ++state->requestCount_;
        return true;
    } else {
        return false;
    }
}

bool rs::jsapi::ContextState::EndRequest(JSContext* cx) {
    auto state = GetState(cx);
    if (state != nullptr) {
        if (state->requestCount_ > 0) {
            --state->requestCount_;
            JS_EndRequest(cx);
        }
        return true;
    } else {
        return false;
    }
}

bool rs::jsapi::ContextState::FlushRequests(JSContext* cx) {
    auto state = GetState(cx);
    if (state != nullptr) {
        for (; state->requestCount_ > 0; --state->requestCount_) {
            JS_EndRequest(cx);
        }
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