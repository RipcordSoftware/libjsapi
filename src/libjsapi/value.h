#ifndef RS_JSAPI_VALUE_H
#define RS_JSAPI_VALUE_H

#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {

class Context;
class Value;
    
class Value {
public:
    Value(Context& cx);
    
    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;    
    
    JS::RootedValue& operator()() { return value_; }

    std::string ToString();

protected:
    Context& cx_;
    JS::RootedValue value_;    
};

}}    
    
#endif	/* RS_JSAPI_VALUE_H */

