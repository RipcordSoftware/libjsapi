#include <cstdlib>

#include "libjsapi.h"

int main() {

    rs::jsapi::Runtime rt;
    
    auto cx = rt.getContext();
    auto global = rt.getGlobal();
    
    const char js[] = "(function(){return {status:'sleeping...'};})();";
    JS::CompileOptions options(cx);    
    auto script = JS_CompileScript(cx, global, js, ::strlen(js), options);
    
    JS::RootedScript s(cx, script);
    JS::RootedValue v(cx);
    JS::MutableHandleValue result(&v);
    JS_ExecuteScript(cx, global, s, result);
    
    return 0;
}

