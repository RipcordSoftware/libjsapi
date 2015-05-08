#ifndef RS_JSAPI_OBJECT_H
#define	RS_JSAPI_OBJECT_H

#include <jsapi.h>

#include <functional>
#include <unordered_map>
#include <vector>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class Object final {
public:
    typedef std::function<void(const char* name, Value& value)> GetCallback;
    typedef std::function<void(const char* name, const Value& value)> SetCallback;
    typedef std::function<void()> FinalizeCallback;
    typedef std::function<void(const std::vector<Value>&, Value&)> FunctionCallback;
    typedef std::unordered_map<std::string, FunctionCallback> Functions;
    
    Object(Context& cx) = delete;
    Object(const Object&) = delete;
    
    static bool Create(Context& cx, 
        const std::vector<const char*>& properties,
        GetCallback getter, SetCallback setter,
        const std::vector<std::pair<const char*, FunctionCallback>>& functions,
        FinalizeCallback finalizer,
        Value& obj);
    
    static bool SetPrivate(Value&, uint64_t, void*);
    static bool GetPrivate(const Value&, uint64_t&, void*&);
    
private:
    struct ObjectState { 
        GetCallback getter; 
        SetCallback setter; 
        FinalizeCallback finalizer; 
        Functions functions;
        uint64_t data;
        void* ptr;
    };
    
    static bool Get(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool Set(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);
    static bool CallFunction(JSContext*, unsigned, JS::Value*);
    static void Finalize(JSFreeOp* fop, JSObject* obj);

    static ObjectState* GetState(JSObject* obj);
    static void SetState(JSObject* obj, ObjectState* state);
    
    static JSClass class_;
};

}}

#endif	/* RS_JSAPI_OBJECT_H */

