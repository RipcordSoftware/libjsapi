#ifndef RS_JSAPI_SCRIPT_H
#define RS_JSAPI_SCRIPT_H

#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class Script final {
public:
    Script(Context& cx, const char* code);
    Script(const Script&) = delete;
    ~Script();
    
    Script& operator=(const Script&) = delete;
    
    void Compile();
    bool Execute();
    bool Execute(Value& result);
    
private:
    Context& cx_;
    const std::string code_;
    
    JS::RootedScript script_;
};

}}

#endif	/* RS_JSAPI_SCRIPT_H */