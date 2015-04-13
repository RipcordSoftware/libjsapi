#include "result.h"

rs::jsapi::Result::Result(Context& cx) : Value(cx), mutableValue_(&value_) {
    
}