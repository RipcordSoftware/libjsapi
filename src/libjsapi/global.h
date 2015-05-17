#ifndef RS_JSAPI_GLOBAL_H
#define	RS_JSAPI_GLOBAL_H

#include <jsapi.h>

#include <functional>
#include <vector>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class Global final {
public:
    typedef std::function<void(const std::vector<Value>&, Value&)> FunctionCallback;
    
    Global() = delete;
    Global(const Global& orig) = delete;
    
    static bool DefineProperty(Context& cx, const char* name, const Value& value, unsigned attrs = JSPROP_ENUMERATE);
    static bool DefineProperty(Context& cx, const char* name, JSNative getter, JSNative setter, unsigned attrs = JSPROP_ENUMERATE);
    static bool DefineFunction(Context& cx, const char* name, FunctionCallback callback, unsigned attrs = JSPROP_ENUMERATE);
    
    static bool DeleteProperty(Context& cx, const char* name);
    static bool DeleteFunction(Context& cx, const char* name);
    
private:
    struct GlobalFunctionState { 
        GlobalFunctionState(FunctionCallback function) : function_(function) {}
        
        const FunctionCallback function_;
    };

    static bool CallFunction(JSContext*, unsigned, JS::Value*);
    static void Finalize(JSFreeOp* fop, JSObject* obj);
    
    static GlobalFunctionState* GetFunctionState(JSObject* obj, JSContext* cx, const char* propName);
    static GlobalFunctionState* GetFunctionState(JSObject* obj);
    static void SetFunctionState(JSObject* obj, GlobalFunctionState* state);
    
    static JSClass privateFunctionStateClass_;
    static const char* privateFunctionStatePropertyName_;
};

}}

#endif	/* RS_JSAPI_GLOBAL_H */

