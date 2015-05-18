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
