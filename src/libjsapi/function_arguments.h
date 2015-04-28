#ifndef RS_JSAPI_FUNCTION_ARGUMENTS_H
#define	RS_JSAPI_FUNCTION_ARGUMENTS_H

#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {

class Context;
class Value;
class Object;
    
class FunctionArguments final {
public:
    FunctionArguments(Context& cx);    
    
    bool Append(signed value);
    bool Append(unsigned value);
    bool Append(double value);
    bool Append(const char* value);
    bool Append(const std::string& value);
    bool Append(bool value);
    bool Append(Value& value);
    bool Append(const JS::RootedObject& obj);
    
    void Clear();
    bool Empty();
    int getLength();
    
    operator const JS::AutoValueVector&() const { return args_; }
    operator const JS::HandleValueArray() const { return args_; }
    JS::Value& operator [](int index);
    
private:
    Context& cx_;
    JS::AutoValueVector args_;
};

}}

#endif	/* RS_JSAPI_FUNCTION_ARGUMENTS_H */

