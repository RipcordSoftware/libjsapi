#ifndef RS_JSAPI_DYNAMIC_OBJECT_H
#define RS_JSAPI_DYNAMIC_OBJECT_H

#include <functional>
#include <vector>
#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class DynamicObject final {
public:
    typedef std::function<bool(const char* name, Value& value)> GetCallback;
    typedef std::function<bool(const char* name, const Value& value)> SetCallback;
    typedef std::function<bool(std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs)> EnumeratorCallback;
    typedef std::function<void()> FinalizeCallback;
    
    static bool Create(Context& cx, GetCallback getter, SetCallback setter, EnumeratorCallback enumerator, FinalizeCallback finalize, Value& obj);
    
private:
    struct ClassCallbacks { GetCallback getter; SetCallback setter; EnumeratorCallback enumerator; FinalizeCallback finalize; };
    
    static bool Get(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool Set(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);
    static bool Enumerate(JSContext* cx, JS::HandleObject obj);
    static void Finalize(JSFreeOp* fop, JSObject* obj);
    
    static ClassCallbacks* GetObjectCallbacks(JSObject* obj);
    static void SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks);
    
    static JSClass class_;
};

}}

#endif