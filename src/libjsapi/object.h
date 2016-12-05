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

#ifndef RS_JSAPI_OBJECT_H
#define	RS_JSAPI_OBJECT_H

#include <jsapi.h>

#include <functional>
#include <unordered_map>
#include <string>
#include <vector>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class Object final {
public:
    using GetCallback = std::function<void(const char* name, Value& value)>;
    using SetCallback = std::function<void(const char* name, const Value& value)>;
    using FinalizeCallback = std::function<void()>;
    using FunctionCallback = std::function<void(const std::vector<Value>&, Value&)>;
    using Functions = std::unordered_map<std::string, FunctionCallback>;
    
    Object(Context& cx) = delete;
    Object(const Object&) = delete;
    
    static bool Create(JSContext*, 
        const std::vector<const char*>& properties,
        const GetCallback& getter, const SetCallback& setter,
        const std::vector<std::pair<const char*, FunctionCallback>>& functions,
        const FinalizeCallback& finalizer,
        Value& obj);
    
    static bool SetPrivate(Value&, uint64_t, void*);
    static bool GetPrivate(const Value&, uint64_t&, void*&);
    
    static bool IsObject(const Value&);
    
private:
    struct ObjectState { 
        GetCallback getter; 
        SetCallback setter; 
        FinalizeCallback finalizer; 
        Functions functions;
        uint64_t data;
        void* ptr;
    };
    
    static bool Get(JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);
    static bool Set(JSContext*, JS::HandleObject, JS::HandleId, bool, JS::MutableHandleValue);
    static bool CallFunction(JSContext*, unsigned, JS::Value*);
    static void Finalize(JSFreeOp* fop, JSObject* obj);

    static ObjectState* GetState(JSContext* cx, JS::HandleObject obj);
    static ObjectState* GetState(JSObject* obj);
    static void SetState(JSObject* obj, ObjectState* state);
    
    static JSClass class_;
};

}}

#endif	/* RS_JSAPI_OBJECT_H */

