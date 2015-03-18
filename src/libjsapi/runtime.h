#ifndef RS_JSAPI_RUNTIME_H
#define RS_JSAPI_RUNTIME_H

#include <jsapi.h>

#include <atomic>
#include <mutex>
#include <memory>

#include "exceptions.h"
#include "context.h"

namespace rs {
namespace jsapi {

class Runtime final {
public:
    Runtime();
    Runtime(const Runtime&) = delete;
    ~Runtime();
    
    Runtime& operator=(const Runtime&) = delete;
    
    std::unique_ptr<Context> NewContext();
    
    JSRuntime* getRuntime();
    Context& getContext();
    
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
    Context cx_;
};

}}

#endif	/* RS_JSAPI_RUNTIME_H */

