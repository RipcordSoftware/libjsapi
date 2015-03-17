#ifndef RS_JSAPI_RUNTIME_H
#define RS_JSAPI_RUNTIME_H

#include <jsapi.h>

#include <atomic>
#include <mutex>
#include <memory>

#include "context.h"

namespace rs {
namespace jsapi {
    
class Context;

class Runtime final {
public:
    Runtime();
    Runtime(const Runtime&) = delete;
    ~Runtime();
    
    Runtime& operator=(const Runtime&) = delete;
    
    std::unique_ptr<Context> NewContext();
    
    JSRuntime* getRuntime() { return rt_; }
    Context& getContext() { return cx_; }
    
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
    
    Instance inst_;
    JSRuntime* rt_;
    rs::jsapi::Context cx_;
};

}}

#endif	/* RS_JSAPI_RUNTIME_H */

