/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Ripcord Software Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/

#ifndef RS_JSAPI_FUNCTION_ARGUMENTS_H
#define	RS_JSAPI_FUNCTION_ARGUMENTS_H

#include <string>

#include <jsapi.h>

#include "value.h"

namespace rs {
namespace jsapi {

class Context;
    
class FunctionArguments final {
public:
    FunctionArguments(Context& cx);    
    
    bool Append(signed value);
    bool Append(unsigned value);
    bool Append(double value);
    bool Append(const char* value);
    bool Append(const std::string& value);
    bool Append(bool value);
    bool Append(const Value& value);
    bool Append(const JS::RootedObject& obj);
    bool Append(const JS::RootedValue& value);
    
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

