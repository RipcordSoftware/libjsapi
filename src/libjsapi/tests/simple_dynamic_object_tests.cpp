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

#include <gtest/gtest.h>

#include "../libjsapi.h"

class SimpleDynamicObjectTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
};

class SimpleDynamicObjectTestException : public std::exception {
public:
    SimpleDynamicObjectTestException(const char* msg) : msg_(msg) {
    }
    
    const char* what() const throw() override { return msg_.c_str(); }
    
private:
    const std::string msg_;
};

TEST_F(SimpleDynamicObjectTests, test1) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        [](const char* name, rs::jsapi::Value& value) {            
            value = 42;
        }, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);
    
    rs::jsapi::Value result(context);
    context.Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(42, result.toInt32());
}

TEST_F(SimpleDynamicObjectTests, test2) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        [](const char* name, rs::jsapi::Value& value) {            
            value = "world";
        }, 
        nullptr,
        nullptr, 
        nullptr,
        obj);            
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);
    
    rs::jsapi::Value result(context);
    context.Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("world", result.ToString().c_str());
}

TEST_F(SimpleDynamicObjectTests, test3) {
    std::string longFieldName(384, '1');
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        [](const char* name, rs::jsapi::Value& value) {            
            value = 42;
        }, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    std::stringstream script;
    script << "var myfunc=function(o){ return o['" << longFieldName << "'];};";
    context.Evaluate(script.str().c_str());
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);
    
    rs::jsapi::Value result(context);
    context.Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(42, result.toInt32());
}

TEST_F(SimpleDynamicObjectTests, test4) {
    rs::jsapi::Context context;
    rs::jsapi::Value fieldValue(context);
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        nullptr, 
        [&](const char* name, const rs::jsapi::Value& value) { 
            fieldValue.set(value); 
        },
        nullptr,
        nullptr,
        obj);        
    ASSERT_TRUE(!!obj);
        
    context.Evaluate("var myfunc=function(o){ o.test = 'world';};");
    
    rs::jsapi::FunctionArguments args(context);
    args.Append(obj);
    
    context.Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());
}

TEST_F(SimpleDynamicObjectTests, test5) {
    rs::jsapi::Context context;
    std::string longFieldName(384, '1');
    rs::jsapi::Value fieldValue(context);
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        nullptr, 
        [&](const char* name, const rs::jsapi::Value& value) { 
            fieldValue.set(value); 
        },
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    std::stringstream script;
    script << "var myfunc=function(o){ o['" << longFieldName << "'] = 'world';};";
    context.Evaluate(script.str().c_str());
    
    rs::jsapi::FunctionArguments args(context);
    args.Append(obj);
    
    context.Call("myfunc", args);
    
    ASSERT_TRUE(fieldValue.isString());
    ASSERT_STREQ("world", fieldValue.ToString().c_str());
}

TEST_F(SimpleDynamicObjectTests, test6) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        nullptr, 
        nullptr, 
        [](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs) {
            props.push_back("the_answer");
            props.push_back("pi");
            props.push_back("hello");
            return true;
        },
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(o){var fields = 0; for (var prop in o) { ++fields; } return fields;};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);
    
    rs::jsapi::Value result(context);
    context.Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(3, result.toInt32());
}

TEST_F(SimpleDynamicObjectTests, test7) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        nullptr, 
        nullptr, 
        [](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs) {
            props.push_back("the_answer");
            props.push_back("pi");
            funcs.push_back(std::make_pair("square", nullptr));
            funcs.push_back(std::make_pair("echo", nullptr));
            return true;
        },
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(o){var fields = 0; for (var prop in o) { ++fields; } return fields;};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);
    
    rs::jsapi::Value result(context);
    context.Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_FLOAT_EQ(4, result.toInt32());
}

TEST_F(SimpleDynamicObjectTests, test8) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        nullptr, 
        nullptr, 
        [](std::vector<std::string>& props, std::vector<std::pair<std::string, JSNative>>& funcs) {
            props.push_back("the_answer");
            props.push_back("pi");
            funcs.push_back(std::make_pair("square", nullptr));
            funcs.push_back(std::make_pair("echo", nullptr));
            return true;
        },
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(o){var fields = []; for (var prop in o) { fields.push(prop); } return fields.toString();};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);
    
    rs::jsapi::Value result(context);
    context.Call("myfunc", args, result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("the_answer,pi,square,echo", result.ToString().c_str());
}

TEST_F(SimpleDynamicObjectTests, test9) {    
    auto count = 0;
    
    if (true) {
        rs::jsapi::Context context;
        
        rs::jsapi::Value obj(context);
        rs::jsapi::DynamicObject::Create(
            context, 
            nullptr, 
            nullptr, 
            nullptr,
            [&]() { ++count; },
            obj);
        ASSERT_TRUE(!!obj);
    }
    
    ASSERT_EQ(1, count);
}

TEST_F(SimpleDynamicObjectTests, test10) {    
    rs::jsapi::Context context;

    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(context, 
        nullptr, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    rs::jsapi::DynamicObject::SetPrivate(obj, 123456789, this);
    
    uint64_t data = 0;
    void* that = nullptr;
    ASSERT_TRUE(rs::jsapi::DynamicObject::GetPrivate(obj, data, that));
    ASSERT_EQ(123456789, data);
    ASSERT_EQ(this, that);
}

TEST_F(SimpleDynamicObjectTests, test11) {    
    rs::jsapi::Context context;

    rs::jsapi::Value obj(context);
    ASSERT_FALSE(rs::jsapi::DynamicObject::SetPrivate(obj, 123456789, this));
    
    uint64_t data = 0;
    void* that = nullptr;
    ASSERT_FALSE(rs::jsapi::DynamicObject::GetPrivate(obj, data, that));
    ASSERT_NE(123456789, data);
    ASSERT_NE(this, that);
}

TEST_F(SimpleDynamicObjectTests, test12) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        [](const char* name, rs::jsapi::Value& value) {            
            throw SimpleDynamicObjectTestException("It happened!");
        }, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);            
    
    ASSERT_THROW({
        rs::jsapi::Value result(context);
        context.Call("myfunc", args, result);
    }, rs::jsapi::ScriptException);    
}

TEST_F(SimpleDynamicObjectTests, test13) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        [](const char* name, rs::jsapi::Value& value) {            
            throw SimpleDynamicObjectTestException("It happened!");
        }, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(n){return n.hello;};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);            
    
    bool thrown = false;
    
    try {
        rs::jsapi::Value result(context);
        context.Call("myfunc", args, result);
    } catch (const rs::jsapi::ScriptException& ex) {
        thrown = true;
    }
    
    ASSERT_TRUE(thrown);
}

TEST_F(SimpleDynamicObjectTests, test14) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        nullptr,
        [](const char* name, const rs::jsapi::Value& value) {            
            throw SimpleDynamicObjectTestException("It happened!");
        },  
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(n){n.hello=null;};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);            
    
    ASSERT_THROW({
        rs::jsapi::Value result(context);
        context.Call("myfunc", args, result);
    }, rs::jsapi::ScriptException);    
}

TEST_F(SimpleDynamicObjectTests, test15) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        nullptr,
        [](const char* name, const rs::jsapi::Value& value) {            
            throw SimpleDynamicObjectTestException("It happened!");
        }, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);
    
    context.Evaluate("var myfunc=function(n){n.hello=null;};");    
    
    rs::jsapi::FunctionArguments args(context);    
    args.Append(obj);            
    
    bool thrown = false;
    
    try {
        rs::jsapi::Value result(context);
        context.Call("myfunc", args, result);
    } catch (const rs::jsapi::ScriptException& ex) {
        thrown = true;
    }
    
    ASSERT_TRUE(thrown);
}

TEST_F(SimpleDynamicObjectTests, test16) {
    rs::jsapi::Context context;
    
    rs::jsapi::Value obj(context);
    rs::jsapi::DynamicObject::Create(
        context,
        [](const char* name, rs::jsapi::Value& value) {            
            value = 42;
        }, 
        nullptr, 
        nullptr,
        nullptr,
        obj);
    ASSERT_TRUE(!!obj);   
    ASSERT_TRUE(rs::jsapi::DynamicObject::IsDynamicObject(obj));
    
    rs::jsapi::Value nullObj(context, JS::NullHandleValue);    
    ASSERT_FALSE(rs::jsapi::DynamicObject::IsDynamicObject(nullObj));
    
    rs::jsapi::Value undefinedObj(context, JS::UndefinedHandleValue);    
    ASSERT_FALSE(rs::jsapi::DynamicObject::IsDynamicObject(undefinedObj));
    
    rs::jsapi::Value numValue(context, 123);    
    ASSERT_FALSE(rs::jsapi::DynamicObject::IsDynamicObject(numValue));
    
    rs::jsapi::Value strValue(context, "123");    
    ASSERT_FALSE(rs::jsapi::DynamicObject::IsDynamicObject(strValue));
    
    rs::jsapi::Value array(context);
    rs::jsapi::DynamicArray::Create(context,
        [](int index, rs::jsapi::Value& value) { value = 42; },
        nullptr,
        []() { return 1; },
        nullptr,
        array);
    ASSERT_TRUE(!!array); 
    ASSERT_FALSE(rs::jsapi::DynamicObject::IsDynamicObject(array));
}
