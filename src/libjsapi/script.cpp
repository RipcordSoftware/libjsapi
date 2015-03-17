#include "script.h"

rs::jsapi::Script::Script(Context& cx, const char* code) : 
    cx_(cx), code_(code), script_(cx) {
}

rs::jsapi::Script::Script(Runtime& rt, const char* code) : 
    cx_(rt.getContext()), code_(code), script_(rt.getContext()) {
}

rs::jsapi::Script::~Script() {
}

void rs::jsapi::Script::Compile() {
    JS::CompileOptions options(cx_);
    script_ = JS_CompileScript(cx_, cx_.getGlobal(), code_.c_str(), code_.length(), options);        
}

bool rs::jsapi::Script::Execute() {
    auto status = JS_ExecuteScript(cx_, cx_.getGlobal(), script_);
    return status;
}

bool rs::jsapi::Script::Execute(Value& result) {
    auto status = JS_ExecuteScript(cx_, cx_.getGlobal(), script_, result);    
    return status;
}