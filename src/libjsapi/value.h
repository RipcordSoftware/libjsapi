#ifndef RS_JSAPI_VALUE_H
#define RS_JSAPI_VALUE_H

#include <string>

#include <jsapi.h>

#include "context.h"
#include "runtime.h"

namespace rs {
namespace jsapi {
    
class Value final {
public:
    Value(Context& cx) : cx_(cx), value_(cx), mutableValue_(&value_) {
    };
    
    Value(Runtime& rt) : cx_(rt.getContext()), value_(rt.getContext()), mutableValue_(&value_) {
    };
    
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

