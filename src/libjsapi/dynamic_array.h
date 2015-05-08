#ifndef RS_JSAPI_DYNAMIC_ARRAY_H
#define RS_JSAPI_DYNAMIC_ARRAY_H

#include <functional>
#include <vector>
#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class DynamicArray final {
public:
    typedef std::function<void(int index, Value& value)> GetCallback;
    typedef std::function<void(int index, const Value& value)> SetCallback;
    typedef std::function<int()> LengthCallback;
    typedef std::function<void()> FinalizeCallback;
    
    static bool Create(Context& cx, GetCallback getter, SetCallback setter, LengthCallback length, FinalizeCallback finalize, Value& array);
    
    static bool SetPrivate(Value&, uint64_t, void*);
    static bool GetPrivate(const Value&, uint64_t&, void*&);
    
private:
    struct DynamicArrayState { GetCallback getter; SetCallback setter; LengthCallback length; FinalizeCallback finalize; uint64_t data; void* ptr; };
    
    static bool Get(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool Set(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);
    static void Finalize(JSFreeOp* fop, JSObject* obj);
    static bool Length(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    
    static DynamicArrayState* GetState(JSObject* obj);
    static void SetState(JSObject* obj, DynamicArrayState* state);
    
    static JSClass class_;
};

}}

#endif