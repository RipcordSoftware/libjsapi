#ifndef RS_JSAPI_VALUE_H
#define RS_JSAPI_VALUE_H

#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {

class Context;
class Runtime;
class Value;
    
class Value final {
public:
    Value(Context& cx);    
    Value(Runtime& rt);
    
    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;
    
    operator JS::MutableHandleValue&() { return mutableValue_; }
    
    JS::RootedValue& operator()() { return value_; }

    std::string ToString();

private:
    Context& cx_;
    JS::RootedValue value_;
    JS::MutableHandleValue mutableValue_;
};

}}    
    
#endif	/* RS_JSAPI_VALUE_H */

