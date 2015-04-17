#ifndef RS_JSAPI_GLOBAL_H
#define	RS_JSAPI_GLOBAL_H

#include <jsapi.h>

namespace rs {
namespace jsapi {
    
class Context;

class Global final {
public:
    Global() = delete;
    Global(const Global& orig) = delete;
    
    static bool DefineProperty(Context& cx, const char* name, JSNative getter, JSNative setter, unsigned attrs = JSPROP_ENUMERATE);
    static bool DefineFunction(Context& cx, const char* name, JSNative func, unsigned attrs = JSPROP_ENUMERATE);
    
private:

};

}}

#endif	/* RS_JSAPI_GLOBAL_H */

