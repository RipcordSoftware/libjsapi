#ifndef RS_JSAPI_DYNAMIC_OBJECT_H
#define RS_JSAPI_DYNAMIC_OBJECT_H

#include <functional>

#include <jsapi.h>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class DynamicObject final {
public:
    typedef std::function<bool(const char* name, Value& value)> Getter;
    typedef std::function<bool(const char* name, const Value& value)> Setter;        
    
    static bool Create(Context& cx, Getter getter, Setter setter, JS::RootedObject& obj);
    
private:
    struct ClassCallbacks { Getter getter; Setter setter; };
    
    static bool GetCallback(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool SetCallback(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);
    static void FinalizeCallback(JSFreeOp* fop, JSObject* obj);
    static ClassCallbacks* GetObjectCallbacks(JSObject* obj);
    static void SetObjectCallbacks(JSObject* obj, ClassCallbacks* callbacks);
    
    static JSClass class_;
};

}}

#endif