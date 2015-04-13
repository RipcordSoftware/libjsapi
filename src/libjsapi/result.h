#ifndef RS_JSAPI_RESULT_H
#define	RS_JSAPI_RESULT_H

#include "value.h"

namespace rs {
namespace jsapi {

class Result final : public Value {
public:
    Result(Context& cx);
    
    operator JS::MutableHandleValue&() { return mutableValue_; }
    
private:
    JS::MutableHandleValue mutableValue_;
};

}}

#endif	/* RS_JSAPI_RESULT_H */

