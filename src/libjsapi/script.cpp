#include "script.h"

#include "context.h"
#include "result.h"

rs::jsapi::Script::Script(Context& cx, const char* code) : 
    cx_(cx), code_(code), script_(cx) {
}

rs::jsapi::Script::~Script() {
}

void rs::jsapi::Script::Compile() {
    JS::CompileOptions options(cx_);
    script_ = JS_CompileScript(cx_, cx_.getGlobal(), code_.c_str(), code_.length(), options);
    
    auto error = cx_.getError();
    if (!!error) {
        throw *error;
    }
}

bool rs::jsapi::Script::Execute() {
    auto status = JS_ExecuteScript(cx_, cx_.getGlobal(), script_);
    
    auto error = cx_.getError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}

bool rs::jsapi::Script::Execute(Result& result) {
    auto status = JS_ExecuteScript(cx_, cx_.getGlobal(), script_, result);    
    
    auto error = cx_.getError();
    if (!!error) {
        throw *error;
    }
    
    return status;
}