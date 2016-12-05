/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Ripcord Software Ltd
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

#ifndef RS_JSAPI_DYNAMIC_ARRAY_H
#define RS_JSAPI_DYNAMIC_ARRAY_H

#include <functional>
#include <vector>
#include <string>

#include <jsapi.h>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class DynamicArray final {
public:
    typedef std::function<void(int index, Value& value)> GetCallback;
    typedef std::function<void(int index, const Value& value)> SetCallback;
    typedef std::function<int()> LengthCallback;
    typedef std::function<void()> FinalizeCallback;
    
    static bool Create(JSContext*, const GetCallback& getter, const SetCallback& setter, const LengthCallback& length, const FinalizeCallback& finalize, Value& array);
    
    static bool SetPrivate(Value&, uint64_t, void*);
    static bool GetPrivate(const Value&, uint64_t&, void*&);
    
    static bool IsDynamicArray(const Value&);
    
private:
    struct DynamicArrayState { GetCallback getter; SetCallback setter; LengthCallback length; FinalizeCallback finalize; uint64_t data; void* ptr; };
    
    static bool Get(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool Set(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);
    static void Finalize(JSFreeOp* fop, JSObject* obj);
    static bool Length(JSContext*, unsigned, JS::Value*);
    
    static DynamicArrayState* GetState(JSContext* cx, JS::HandleObject obj);
    static DynamicArrayState* GetState(JSObject* obj);
    static void SetState(JSObject* obj, DynamicArrayState* state);
    
    static JSClass class_;
};

}}

#endif