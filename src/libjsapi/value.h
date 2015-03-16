#ifndef RS_JSAPI_VALUE_H
#define RS_JSAPI_VALUE_H

#include <jsapi.h>

#include "runtime.h"

namespace rs {
namespace jsapi {
    
class Value final {
public:
    Value(Runtime& rt) : value_(rt.getContext()), mutableValue_(&value_) {
    };
    
    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;
    
    operator JS::MutableHandleValue&() { return mutableValue_; }
    
    JS::RootedValue& operator()() { return value_; }

private:
    JS::RootedValue value_;
    JS::MutableHandleValue mutableValue_;
};

}}    
    
#endif	/* RS_JSAPI_VALUE_H */

