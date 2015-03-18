#ifndef RS_JSAPI_CONTEXT_H
#define RS_JSAPI_CONTEXT_H

#include <string>
#include <memory>

#include <jsapi.h>

#include "exceptions.h"
#include "script.h"

namespace rs {
namespace jsapi {

class Runtime;
    
class Context final {
    friend class Runtime;
public:
    Context(Runtime& rt);    
    ~Context();
    
    Context(const Context&) = delete;
    Context& operator =(const Context&) = delete;
    
    JSContext* getContext() { return cx_; }
    JS::HandleObject getGlobal() { return global_; }
    
    operator JSContext*() { return cx_; }        
        
private:
    friend void Script::Compile();
    friend bool Script::Execute();
    friend bool Script::Execute(Value& result);
    
    std::unique_ptr<ScriptException> getError();
    
    static void ReportError(JSContext *cx, const char *message, JSErrorReport *report);
    
    void DestroyContext();
   
    JSContext* cx_;
    JS::RootedObject global_;
    JSCompartment* oldCompartment_;
    
    std::unique_ptr<ScriptException> exception_;
};

}}

#endif	/* RS_JSAPI_CONTEXT_H */