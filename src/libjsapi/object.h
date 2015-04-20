#ifndef RS_JSAPI_OBJECT_H
#define	RS_JSAPI_OBJECT_H

#include <jsapi.h>

#include <initializer_list>
#include <functional>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class Object final {
public:
    typedef std::function<bool(const char* name, Value& value)> Getter;
    typedef std::function<bool(const char* name, const Value& value)> Setter;
    typedef std::pair<const char*, JSNative> Function;
    
    Object(Context& cx) = delete;
    Object(const Object&) = delete;
    
    static bool Create(Context& cx, 
        std::initializer_list<const char*> properties,
        Getter getter, Setter setter,
        std::initializer_list<Function> functions,
        JS::RootedObject& obj);
    
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

#endif	/* RS_JSAPI_OBJECT_H */

