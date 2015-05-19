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

#ifndef RS_JSAPI_GLOBAL_H
#define	RS_JSAPI_GLOBAL_H

#include <jsapi.h>

#include <functional>
#include <vector>

namespace rs {
namespace jsapi {
    
class Context;
class Value;

class Global final {
public:
    typedef std::function<void(const std::vector<Value>&, Value&)> FunctionCallback;
    
    Global() = delete;
    Global(const Global& orig) = delete;
    
    static bool DefineProperty(Context& cx, const char* name, const Value& value, unsigned attrs = JSPROP_ENUMERATE);
    static bool DefineProperty(Context& cx, const char* name, JSNative getter, JSNative setter, unsigned attrs = JSPROP_ENUMERATE);
    static bool DefineFunction(Context& cx, const char* name, FunctionCallback callback, unsigned attrs = JSPROP_ENUMERATE);
    
    static bool DeleteProperty(Context& cx, const char* name);
    static bool DeleteFunction(Context& cx, const char* name);
    
private:
    struct GlobalFunctionState { 
        GlobalFunctionState(FunctionCallback function) : function_(function) {}
        
        const FunctionCallback function_;
    };

    static bool CallFunction(JSContext*, unsigned, JS::Value*);
    static void Finalize(JSFreeOp* fop, JSObject* obj);
    
    static GlobalFunctionState* GetFunctionState(JSObject* obj, JSContext* cx, const char* propName);
    static GlobalFunctionState* GetFunctionState(JSObject* obj);
    static void SetFunctionState(JSObject* obj, GlobalFunctionState* state);
    
    static JSClass privateFunctionStateClass_;
    static const char* privateFunctionStatePropertyName_;
};

}}

#endif	/* RS_JSAPI_GLOBAL_H */

