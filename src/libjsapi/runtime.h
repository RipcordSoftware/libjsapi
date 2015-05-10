#ifndef RS_JSAPI_RUNTIME_H
#define RS_JSAPI_RUNTIME_H

#include <jsapi.h>

#include <atomic>
#include <mutex>
#include <memory>
#include <thread>
#include <vector>

#include "exceptions.h"
#include "context.h"

namespace rs {
namespace jsapi {
    
class Value;    
class FunctionArguments;

class Runtime final {
public:
    Runtime(uint32_t maxBytes = 64 * 1024 * 1024, bool enableBaselineCompiler = true, bool enableIonCompiler = false);
    Runtime(const Runtime&) = delete;
    ~Runtime();
    
    bool Evaluate(const char* script);
    bool Evaluate(const char* script, Value& result);
    bool Call(const char* name);
    bool Call(const char* name, const FunctionArguments& args);
    bool Call(const char* name, Value& result);
    bool Call(const char* name, const FunctionArguments& args, Value& result);
    
    Runtime& operator=(const Runtime&) = delete;
    
    std::unique_ptr<Context> NewContext();
    
    JSRuntime* getRuntime();
    Context& getContext() { return cx_; }
    
    operator Context&() { 
        if (threadId_ != std::this_thread::get_id()) {
            throw RuntimeWrongThreadException();
        }
        
        return cx_; 
    }
    
    operator JSContext*() { 
        if (threadId_ != std::this_thread::get_id()) {
            throw RuntimeWrongThreadException();
        }
        
        return cx_; 
    }
    
private:
    
    class Instance {
    public:
        Instance();
        ~Instance();
        
    private:
        static std::atomic<int> count_;
        static std::mutex m_;
    };  
    
    class RuntimeThreadGuard {
    public:
        RuntimeThreadGuard(std::thread::id id);
    };
    
    static std::vector<std::thread::id> activeRuntimes_;
    static std::mutex activeRuntimesLock_;
    
    const std::thread::id threadId_;
    const RuntimeThreadGuard threadGuard_;    
    Instance inst_;
    JSRuntime* rt_;
    Context cx_;
    
    static bool AddRuntime(std::thread::id id);
    static bool RemoveRuntime(std::thread::id id);
};

}}

#endif	/* RS_JSAPI_RUNTIME_H */

