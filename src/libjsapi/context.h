#ifndef RS_JSAPI_CONTEXT_H
#define RS_JSAPI_CONTEXT_H

#include <string>

#include <jsapi.h>

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
    
    bool HasError();
    const std::string& getError();
    bool ClearError();
        
private:
    static void ReportError(JSContext *cx, const char *message, JSErrorReport *report);
    
    void DestroyContext();
    
    JSContext* cx_;
    JS::RootedObject global_;
    JSAutoCompartment ac_;
    
    std::string error_;
};

}}

#endif	/* RS_JSAPI_CONTEXT_H */

