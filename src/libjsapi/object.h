#ifndef RS_JSAPI_OBJECT_H
#define	RS_JSAPI_OBJECT_H

#include <jsapi.h>

namespace rs {
namespace jsapi {
    
class Context;

class Object final {
public:
    Object(Context& cx);
    Object(Context& cx, JSObject* obj);
    
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    
    bool DefineProperty(const char* name, JSNative getter = nullptr, JSNative setter = nullptr, unsigned attrs = JSPROP_SHARED | JSPROP_ENUMERATE);    
    bool DefineFunction(const char* name, JSNative func, unsigned attrs = JSPROP_SHARED | JSPROP_ENUMERATE);    
    
    operator const JS::HandleObject() const { return obj_; }
    operator const JS::Value() const { return JS::ObjectValue(*obj_); }
    
private:
    Context& cx_;
    JS::RootedObject obj_;
    
};

}}

#endif	/* RS_JSAPI_OBJECT_H */

