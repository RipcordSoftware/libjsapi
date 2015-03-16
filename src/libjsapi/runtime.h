#ifndef RS_JSAPI_RUNTIME_H
#define RS_JSAPI_RUNTIME_H

#include <jsapi.h>

#include <atomic>
#include <mutex>

namespace rs {
namespace jsapi {

class Runtime final {
public:
    Runtime();
    Runtime(const Runtime&) = delete;
    ~Runtime();
    
    Runtime& operator=(const Runtime&) = delete;
    
    JSRuntime* getRuntime() { return rt_; }
    JS::HandleObject getGlobal() { return global_; }
    JSContext* getContext() { return cx_; }
    
    bool HasError();
    const std::string& getError();
    bool ClearError();
    
private:
    
    class Instance {
    public:
        Instance();
        ~Instance();
        
    private:
        static std::atomic<int> count_;
        static std::mutex m_;
    };
    
    static void ReportError(JSContext *cx, const char *message, JSErrorReport *report);    
    
    Instance inst_;
    JSRuntime* rt_;
    JSContext* cx_;
    JS::RootedObject global_;
    JSAutoCompartment ac_;
    
    std::string error_;
};

}}

#endif	/* RS_JSAPI_RUNTIME_H */

