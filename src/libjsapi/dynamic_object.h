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
    typedef std::function<bool(const char* name, Value& value)> Getter;
    typedef std::function<bool(const char* name, const Value& value)> Setter;
    typedef std::function<bool(std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs)> Enumerator;
    
    static bool Create(Context& cx, Getter getter, Setter setter, Enumerator enumerator, JS::RootedObject& obj);
    
private:
    struct ClassCallbacks { Getter getter; Setter setter; Enumerator enumerator; };
    
    static bool GetCallback(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool SetCallback(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);
    static bool EnumerateCallback(JSContext* cx, JS::HandleObject obj);
    static void FinalizeCallback(JSFreeOp* fop, JSObject* obj);
    static ClassCallbacks* GetObjectCallbacks(JSObject* obj);
    static void SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks);
    
    static JSClass class_;
};

}}

#endif