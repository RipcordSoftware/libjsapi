#ifndef RS_JSAPI_SCRIPT_H
#define RS_JSAPI_SCRIPT_H

#include <string>

#include <jsapi.h>

#include "runtime.h"
#include "value.h"

namespace rs {
namespace jsapi {

class Script final {
public:
    Script(Runtime& rt, const char* code);
    Script(const Script&) = delete;
    ~Script();
    
    Script& operator=(const Script&) = delete;
    
    void Compile();
    bool Execute(Value& result);
    
private:
    Runtime& rt_;
    const std::string code_;
    
    JS::RootedScript script_;
};

}}

#endif	/* RS_JSAPI_SCRIPT_H */