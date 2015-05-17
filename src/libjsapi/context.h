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
class FunctionArguments;
class Value;

class Context final {
    friend class Runtime;
public:
    Context(Runtime& rt);    
    ~Context();
    
    bool Evaluate(const char* script);
    bool Evaluate(const char* script, Value& result);
    bool Call(const char* name);
    bool Call(const char* name, const FunctionArguments& args);
    bool Call(const char* name, Value& result);
    bool Call(const char* name, const FunctionArguments& args, Value& result);
    
    Context(const Context&) = delete;
    Context& operator =(const Context&) = delete;
    
    JSContext* getContext() { return cx_; }
    JS::HandleObject getGlobal() { return global_; }
    
    operator JSContext*() { return cx_; }
    
    void MaybeGC() { JS_MaybeGC(cx_); }
        
private:
    friend bool Script::Compile();
    friend bool Script::Execute();
    friend bool Script::Execute(Value&);
    
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