#include "script.h"

rs::jsapi::Script::Script(Runtime& rt, const char* code) : 
    rt_(rt), code_(code), script_(rt.getContext()) {
}

rs::jsapi::Script::~Script() {
}

void rs::jsapi::Script::Compile() {
    JS::CompileOptions options(rt_.getContext());
    script_ = JS_CompileScript(rt_.getContext(), rt_.getGlobal(), code_.c_str(), code_.length(), options);        
}

bool rs::jsapi::Script::Execute(Value& result) {
    auto status = JS_ExecuteScript(rt_.getContext(), rt_.getGlobal(), script_, result);    
    return status;    
}