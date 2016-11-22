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

#include <gtest/gtest.h>

#include "../libjsapi.h"

#include <cstring>
#include <string>
#include <sstream>

class SimpleObjectTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }     
    
    rs::jsapi::Runtime rt_;
};

class SimpleObjectTestException : public std::exception {
public:
    SimpleObjectTestException(const char* msg) : msg_(msg) {
    }
    
    const char* what() const throw() override { return msg_.c_str(); }
    
private:
    const std::string msg_;
};

TEST_F(SimpleObjectTests, test1) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, {}, rs::jsapi::Object::GetCallback(), rs::jsapi::Object::SetCallback(), {}, nullptr, obj);
    ASSERT_TRUE(!!obj);
}

TEST_F(SimpleObjectTests, test2) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, {"hello"}, rs::jsapi::Object::GetCallback(), rs::jsapi::Object::SetCallback(), {}, nullptr, obj);
    ASSERT_TRUE(!!obj);
    
    JS::Rooted<JSPropertyDescriptor> desc(context->getContext());
    JS::MutableHandle<JSPropertyDescriptor> descHandle(&desc);
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "hello", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
}

TEST_F(SimpleObjectTests, test3) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, {"hello", "pi", "lorem", "the_answer"}, rs::jsapi::Object::GetCallback(), rs::jsapi::Object::SetCallback(), {}, nullptr, obj);
    ASSERT_TRUE(!!obj);
    
    JS::Rooted<JSPropertyDescriptor> desc(context->getContext());
    JS::MutableHandle<JSPropertyDescriptor> descHandle(&desc);
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "hello", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "pi", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "lorem", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "the_answer", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
}

TEST_F(SimpleObjectTests, test4) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {"hello", "pi", "lorem", "the_answer"}, 
        [](const char* name, rs::jsapi::Value& value) {
            if (std::strcmp(name, "hello") == 0) {
                value.set("world");
            } else if (std::strcmp(name, "pi") == 0) {
                value.set(3.14159);
            } else if (std::strcmp(name, "lorem") == 0) {
                value.set("Lorem ipsum...");
            } else if (std::strcmp(name, "the_answer") == 0) {
                value.set(42);
            } else {
                value.setUndefined();
            }
        }, 
        nullptr, 
        {},
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::Value value(*context);
    ASSERT_TRUE(JS_GetProperty(*context, obj, "hello", value));
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("world", value.ToString().c_str());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "pi", value));
    ASSERT_TRUE(value.isNumber());
    ASSERT_FLOAT_EQ(3.14159, value.toNumber());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "lorem", value));
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("Lorem ipsum...", value.ToString().c_str());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "the_answer", value));
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "xyz", value));
    ASSERT_TRUE(value.isUndefined());
}

TEST_F(SimpleObjectTests, test5) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {"hello", "pi", "lorem", "the_answer"}, 
        nullptr, 
        nullptr, 
        {},
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::Value value(*context);
    ASSERT_TRUE(JS_GetProperty(*context, obj, "hello", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "pi", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "lorem", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "the_answer", value));
    ASSERT_TRUE(value.isUndefined());
    
    ASSERT_TRUE(JS_GetProperty(*context, obj, "xyz", value));
    ASSERT_TRUE(value.isUndefined());
}

TEST_F(SimpleObjectTests, test5b) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {"hello", "pi", "lorem", "the_answer"}, 
        nullptr, 
        nullptr, 
        {}, 
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    ASSERT_TRUE(JS_SetProperty(*context, obj, "hello", rs::jsapi::Value(rt_, "world")));
    ASSERT_TRUE(JS_SetProperty(*context, obj, "pi", rs::jsapi::Value(rt_, 3.14159)));
    ASSERT_TRUE(JS_SetProperty(*context, obj, "the_answer", rs::jsapi::Value(rt_, 42)));
    ASSERT_TRUE(JS_SetProperty(*context, obj, "lorem", rs::jsapi::Value(rt_, "Lorem ipsum...")));
}

TEST_F(SimpleObjectTests, test6) {
    auto context = rt_.NewContext();
    
    std::string longFieldName(384, '1');
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        { longFieldName.c_str() },
        [](const char* name, rs::jsapi::Value& value) { value = 42; },
        nullptr, 
        {}, 
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::Value value(*context);
    ASSERT_TRUE(JS_GetProperty(*context, obj, longFieldName.c_str(), value));
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
}

TEST_F(SimpleObjectTests, test7) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "the_answer" }, 
        [](const char* name, rs::jsapi::Value& value) { value = 42; },
        nullptr,
        {},
        nullptr, 
        obj);
           
    context->Evaluate("var myfunc=function(n){return n.the_answer;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(SimpleObjectTests, test8) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "hello" }, 
        [](const char* name, rs::jsapi::Value& value) { value = "world"; },
        nullptr,
        {},
        nullptr, 
        obj);
        
    context->Evaluate("var myfunc=function(n){return n.hello;};");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("world", result.ToString().c_str());
}

TEST_F(SimpleObjectTests, test9) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {},
        nullptr,
        nullptr, 
        { std::make_pair("myfunc", [](const std::vector<rs::jsapi::Value>&, rs::jsapi::Value&){} )}, 
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    JS::Rooted<JSPropertyDescriptor> desc(context->getContext());
    JS::MutableHandle<JSPropertyDescriptor> descHandle(&desc);
    ASSERT_TRUE(JS_GetPropertyDescriptor(*context, obj, "myfunc", descHandle));
    ASSERT_TRUE(desc.get().attrs & JSPROP_ENUMERATE != 0);
    ASSERT_TRUE(JS_IsNative(desc.object()));
}

TEST_F(SimpleObjectTests, test10) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {},
        nullptr,
        nullptr, 
        { std::make_pair("myfunc", [](const std::vector<rs::jsapi::Value>&, rs::jsapi::Value& result) { 
            result = 42;
        })}, 
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::FunctionArguments args(*context);
    rs::jsapi::Value result(*context);
    ASSERT_TRUE(JS_CallFunctionName(*context, obj, "myfunc", args, result));
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(SimpleObjectTests, test11) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {},
        nullptr,
        nullptr, 
        { 
            std::make_pair("myfunc", [](const std::vector<rs::jsapi::Value>&, rs::jsapi::Value& result) { 
                result = 42;
            })
        },
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
      
    context->Evaluate("var myfunc = function(o) { return o.myfunc(); }");
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    rs::jsapi::Value result(*context);
    context->Call("myfunc", args, result);    
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(SimpleObjectTests, test12) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value fieldValue(*context);
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "hello" },         
        nullptr,
        [&](const char* name, const rs::jsapi::Value& value) { 
            fieldValue = value;
        },
        {},
        nullptr, 
        obj);
        
    context->Evaluate("var myfunc=function(o){ o.hello = 'world';};");    
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    context->Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());    
}

TEST_F(SimpleObjectTests, test13) {
    auto context = rt_.NewContext();
    
    std::string longFieldName(384, '1');
    rs::jsapi::Value fieldValue(*context);
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { longFieldName.c_str() },         
        nullptr,
        [&](const char* name, const rs::jsapi::Value& value) { 
            fieldValue = value;
        },
        {},
        nullptr, 
        obj);
        
    std::stringstream script;
    script << "var myfunc=function(o){ o['" << longFieldName << "'] = 'world';};";
    context->Evaluate(script.str().c_str());
    
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    context->Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());    
}

TEST_F(SimpleObjectTests, test14) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {"hello", "pi", "lorem", "the_answer"}, 
        nullptr, 
        nullptr, 
        {},
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context->Evaluate("var myfunc = function(o){ var arr=[]; for (var prop in o) { arr.push(prop); } return arr.toString(); };");
    
    rs::jsapi::Value result(*context);
    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    context->Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello,pi,lorem,the_answer", result.ToString().c_str());    
}

TEST_F(SimpleObjectTests, test15) {    
    auto count = 0;
    
    if (true) {
        auto context = rt_.NewContext();
        
        rs::jsapi::Value obj(*context);
        rs::jsapi::Object::Create(*context, 
            {}, 
            nullptr, 
            nullptr, 
            {},
            [&]() { ++count; },
            obj);
        ASSERT_TRUE(!!obj);
    }
    
    ASSERT_EQ(1, count);
}

TEST_F(SimpleObjectTests, test16) {    
    auto context = rt_.NewContext();

    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, 
        {}, 
        nullptr, 
        nullptr, 
        {},
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::Object::SetPrivate(obj, 123456789, this);
    
    uint64_t data = 0;
    void* that = nullptr;
    ASSERT_TRUE(rs::jsapi::Object::GetPrivate(obj, data, that));
    ASSERT_EQ(123456789, data);
    ASSERT_EQ(this, that);
}

TEST_F(SimpleObjectTests, test17) {    
    auto context = rt_.NewContext();

    rs::jsapi::Value obj(*context);
    ASSERT_FALSE(rs::jsapi::Object::SetPrivate(obj, 123456789, this));
    
    uint64_t data = 0;
    void* that = nullptr;
    ASSERT_FALSE(rs::jsapi::Object::GetPrivate(obj, data, that));
    ASSERT_NE(123456789, data);
    ASSERT_NE(this, that);
}

TEST_F(SimpleObjectTests, test18) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "the_answer" }, 
        [](const char* name, rs::jsapi::Value& value) { 
            throw SimpleObjectTestException("It happened!");
        },
        nullptr,
        {},
        nullptr, 
        obj);
        
    context->Evaluate("var myfunc=function(n){return n.the_answer;};");

    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    ASSERT_THROW({        
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);
    }, rs::jsapi::ScriptException);        
}

TEST_F(SimpleObjectTests, test19) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "the_answer" }, 
        [](const char* name, rs::jsapi::Value& value) { 
            throw SimpleObjectTestException("It happened!");
        },
        nullptr,
        {},
        nullptr, 
        obj);
        
    context->Evaluate("var myfunc=function(n){return n.the_answer;};");

    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    bool thrown = false;
    try {
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);
    } catch (const rs::jsapi::ScriptException& ex) {
        thrown = true;
    }
    
    ASSERT_TRUE(thrown);
}

TEST_F(SimpleObjectTests, test20) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "the_answer" }, 
        nullptr,
        [](const char* name, const rs::jsapi::Value& value) { 
            throw SimpleObjectTestException("It happened!");
        },
        {},
        nullptr, 
        obj);
        
    context->Evaluate("var myfunc=function(n){ n.the_answer = 42;};");

    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    ASSERT_THROW({
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);
    }, rs::jsapi::ScriptException);        
}

TEST_F(SimpleObjectTests, test21) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, { "the_answer" }, 
        nullptr,
        [](const char* name, const rs::jsapi::Value& value) { 
            throw SimpleObjectTestException("It happened!");
        },
        {},
        nullptr, 
        obj);
        
    context->Evaluate("var myfunc=function(n){ n.the_answer = 42;};");

    rs::jsapi::FunctionArguments args(*context);
    args.Append(obj);
    
    bool thrown = false;
    try {
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);
    } catch (const rs::jsapi::ScriptException& ex) {
        thrown = true;
    }
    
    ASSERT_TRUE(thrown);
}

TEST_F(SimpleObjectTests, test22) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value obj(*context);
    rs::jsapi::Object::Create(*context, {}, rs::jsapi::Object::GetCallback(), rs::jsapi::Object::SetCallback(), {}, nullptr, obj);
    ASSERT_TRUE(!!obj);
    ASSERT_TRUE(rs::jsapi::Object::IsObject(obj));
    
    rs::jsapi::Value nullObj(*context, JS::NullHandleValue);    
    ASSERT_FALSE(rs::jsapi::Object::IsObject(nullObj));
    
    rs::jsapi::Value undefinedObj(*context, JS::UndefinedHandleValue);    
    ASSERT_FALSE(rs::jsapi::Object::IsObject(undefinedObj));
    
    rs::jsapi::Value numValue(*context, 123);    
    ASSERT_FALSE(rs::jsapi::Object::IsObject(numValue));
    
    rs::jsapi::Value strValue(*context, "123");    
    ASSERT_FALSE(rs::jsapi::Object::IsObject(strValue));
    
    rs::jsapi::Value array(*context);
    rs::jsapi::DynamicArray::Create(*context,
        [](int index, rs::jsapi::Value& value) { value = 42; },
        nullptr,
        []() { return 1; },
        nullptr,
        array);
    ASSERT_TRUE(!!array); 
    ASSERT_FALSE(rs::jsapi::Object::IsObject(array));
    
    rs::jsapi::Value dynObj(*context);
    rs::jsapi::DynamicObject::Create(
        *context,
        [](const char* name, rs::jsapi::Value& value) {            
            value = 42;
        }, 
        nullptr, 
        nullptr,
        nullptr,
        dynObj);
    ASSERT_TRUE(!!dynObj);
    ASSERT_FALSE(rs::jsapi::Object::IsObject(dynObj));
}