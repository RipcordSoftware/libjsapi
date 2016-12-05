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

#ifndef RS_JSAPI_DYNAMIC_OBJECT_H
#define RS_JSAPI_DYNAMIC_OBJECT_H

#include <functional>
#include <vector>
#include <string>

#include <jsapi.h>

#include "value.h"

namespace rs {
namespace jsapi {

class DynamicObject final {
public:
    using GetCallback = std::function<void(const char* name, Value& value)>;
    using SetCallback = std::function<void(const char* name, const Value& value)>;
    using EnumeratorCallback = std::function<bool(std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs)>;
    using FinalizeCallback = std::function<void()>;
    
    static bool Create(JSContext*, const GetCallback& getter, const SetCallback& setter, const EnumeratorCallback& enumerator, const FinalizeCallback& finalize, Value& obj);
    
    static bool SetPrivate(Value&, uint64_t, void*);
    static bool GetPrivate(const Value&, uint64_t&, void*&);
    
    static bool IsDynamicObject(const Value&);
    
private:
    struct DynamicObjectState { GetCallback getter; SetCallback setter; EnumeratorCallback enumerator; FinalizeCallback finalize; uint64_t data; void* ptr; };
    
    static bool Get(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool Set(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue, JS::ObjectOpResult&);
    static bool Enumerate(JSContext* cx, JS::HandleObject obj);
    static void Finalize(JSFreeOp* fop, JSObject* obj);
    
    static bool Get(JSContext*, JS::HandleObject, JSString*, JS::MutableHandleValue);
    static bool Set(JSContext*, JS::HandleObject, JSString*, JS::MutableHandleValue);
    
    static DynamicObjectState* GetState(JSContext* cx, JS::HandleObject obj);
    static DynamicObjectState* GetState(JSObject* obj);
    static void SetState(JSObject* obj, DynamicObjectState* state);
    
    static JSClassOps classOps_;
    static JSClass class_;
};

}}

#endif