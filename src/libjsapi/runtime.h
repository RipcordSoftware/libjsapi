#ifndef RS_JSAPI_RUNTIME_H
#define	RS_JSAPI_RUNTIME_H

#include <jsapi.h>

#include <atomic>
#include <mutex>

namespace rs {
namespace jsapi {

class Runtime {
public:
    Runtime();
    Runtime(const Runtime&) = delete;
    ~Runtime();
    
    Runtime& operator=(const Runtime&) = delete;
    
    JSRuntime* getRuntime() { return rt_; }
    JS::HandleObject getGlobal() { return global_; }
    JSContext* getContext() { return cx_; }
    
private:
    
    class Instance {
    public:
        Instance();
        ~Instance();
        
    private:
        static std::atomic<int> count_;
        static std::mutex m_;
    };
    
    Instance inst_;
    JSRuntime* rt_;
    JSContext* cx_;
    JS::RootedObject global_;
    JSAutoCompartment ac_;
};

}}

#endif	/* RS_JSAPI_RUNTIME_H */

