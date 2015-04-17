#ifndef RS_JSAPI_OBJECT_H
#define	RS_JSAPI_OBJECT_H

#include <jsapi.h>

#include <initializer_list>
#include <functional>

namespace rs {
namespace jsapi {
    
class Context;

class Object final {
public:
    typedef bool (*Getter)(JSContext* cx, const char* name, JS::MutableHandleValue value);
    typedef bool (*Setter)(JSContext* cx, const char* name, JS::MutableHandleValue value);
    typedef std::pair<const char*, JSNative> Function;
    
    Object(Context& cx) = delete;
    Object(const Object&) = delete;
    
    static bool Create(Context& cx, 
        std::initializer_list<const char*> properties,
        Getter getter, Setter setter,
        std::initializer_list<Function> functions,
        JS::RootedObject& obj);
    
private:
    static bool GetCallback(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool SetCallback(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);

    static void SetGetter(JSContext* cx, JS::HandleObject obj, Getter getter);
    static Getter GetGetter(JSContext* cx, JS::HandleObject obj);
    static void SetSetter(JSContext* cx, JS::HandleObject obj, Setter setter);
    static Setter GetSetter(JSContext* cx, JS::HandleObject obj);
};

}}

#endif	/* RS_JSAPI_OBJECT_H */

