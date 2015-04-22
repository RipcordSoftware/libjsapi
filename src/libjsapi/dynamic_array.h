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
    typedef std::function<bool(int index, Value& value)> GetCallback;
    typedef std::function<bool(int index, const Value& value)> SetCallback;
    typedef std::function<int()> LengthCallback;
    typedef std::function<void()> FinalizeCallback;
    
    static bool Create(Context& cx, GetCallback getter, SetCallback setter, LengthCallback length, FinalizeCallback finalize, Value& array);
    
private:
    struct ClassCallbacks { GetCallback getter; SetCallback setter; LengthCallback length; FinalizeCallback finalize; };
    
    static bool Get(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool Set(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);
    static void Finalize(JSFreeOp* fop, JSObject* obj);
    static bool Length(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    
    static ClassCallbacks* GetObjectCallbacks(JSObject* obj);
    static void SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks);
    
    static JSClass class_;
};

}}

#endif