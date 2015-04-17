#include "result.h"

rs::jsapi::Result::Result(Context& cx) : Value(cx), mutableValue_(&value_) {
    
}

void rs::jsapi::Result::Set(const JS::HandleValue& value) {
    mutableValue_.set(value);
}