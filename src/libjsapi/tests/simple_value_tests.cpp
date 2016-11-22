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

#include <string>
#include <vector>
#include <sstream>

class SimpleValueTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }  

    rs::jsapi::Runtime rt_;

public:
    static bool SanityCheckWhatMessage(const char* msg);
};

bool SimpleValueTests::SanityCheckWhatMessage(const char* msg) {
    while (*msg != '\0') {
        if (*msg < 0x0d || *msg > 0x7e) {
            return false;
        }
        msg++;
    }
    return true;
}

TEST_F(SimpleValueTests, test1) {
    rs::jsapi::Value value(rt_);
    
    ASSERT_TRUE(value.isUndefined());
    ASSERT_STREQ("undefined", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test2) {
    rs::jsapi::Value value(rt_, 42);
    
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
    ASSERT_STREQ("42", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test3) {
    rs::jsapi::Value value(rt_, 3.14159);
    
    ASSERT_TRUE(value.isNumber());
    ASSERT_FLOAT_EQ(3.14159, value.toNumber());
    ASSERT_STREQ("3.14159", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test4) {
    rs::jsapi::Value value(rt_, true);
    
    ASSERT_TRUE(value.isBoolean());
    ASSERT_TRUE(value.toBoolean());
    ASSERT_STREQ("true", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test5) {
    rs::jsapi::Value value(rt_, false);
    
    ASSERT_TRUE(value.isBoolean());
    ASSERT_FALSE(value.toBoolean());
    ASSERT_STREQ("false", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test6) {
    rs::jsapi::Value value(rt_, "hello world");
    
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("hello world", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test7) {
    std::string str("hello world");
    rs::jsapi::Value value(rt_, str);
    
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("hello world", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test7b) {
    std::string str(512, '1');
    rs::jsapi::Value value(rt_, str);
    
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ(str.c_str(), value.ToString().c_str());
}

TEST_F(SimpleValueTests, test8) {
    rs::jsapi::Value value(rt_);            
    
    value.set(42);
    ASSERT_TRUE(value.isInt32());
    ASSERT_EQ(42, value.toInt32());
    ASSERT_STREQ("42", value.ToString().c_str());
    
    value.set(3.14159);
    ASSERT_TRUE(value.isNumber());
    ASSERT_FLOAT_EQ(3.14159, value.toNumber());
    ASSERT_STREQ("3.14159", value.ToString().c_str());
    
    value.set(true);
    ASSERT_TRUE(value.isBoolean());
    ASSERT_TRUE(value.toBoolean());
    ASSERT_STREQ("true", value.ToString().c_str());
    
    value.set(false);
    ASSERT_TRUE(value.isBoolean());
    ASSERT_FALSE(value.toBoolean());
    ASSERT_STREQ("false", value.ToString().c_str());
    
    value.setUndefined();
    ASSERT_TRUE(value.isUndefined());
    ASSERT_STREQ("undefined", value.ToString().c_str());
    
    value.set("hello world");
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("hello world", value.ToString().c_str());
    
    value.setNull();
    ASSERT_TRUE(value.isNull());
    ASSERT_STREQ("null", value.ToString().c_str());        
    
    std::string str("hello world");
    value.set(str);    
    ASSERT_TRUE(value.isString());
    ASSERT_STREQ("hello world", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test9) {
    rs::jsapi::Value value(rt_, 42);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("42", ss.str().c_str());
}

TEST_F(SimpleValueTests, test10) {
    rs::jsapi::Value value(rt_, true);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("true", ss.str().c_str());
}

TEST_F(SimpleValueTests, test11) {
    rs::jsapi::Value value(rt_, false);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("false", ss.str().c_str());
}

TEST_F(SimpleValueTests, test12) {
    rs::jsapi::Value value(rt_, 3.14159);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("3.14159", ss.str().c_str());
}

TEST_F(SimpleValueTests, test13) {
    rs::jsapi::Value value(rt_, "hello world");            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ("hello world", ss.str().c_str());
}

TEST_F(SimpleValueTests, test14) {
    std::string str(512, '1');
    rs::jsapi::Value value(rt_, str);            
    
    std::stringstream ss;
    ss << value;        
    ASSERT_STREQ(str.c_str(), ss.str().c_str());
}

TEST_F(SimpleValueTests, test15) {
    rs::jsapi::Value value(rt_, "hello world");            
    
    JS::RootedString rv(rt_.getRuntime(), value.toString());
    auto len = JS_GetStringLength(rv);
    ASSERT_EQ(11, len);
    
    std::vector<char> buffer(len);
    JS_EncodeStringToBuffer(rt_, rv, &buffer[0], len);
    std::string str(&buffer[0], len);
    ASSERT_STREQ("hello world", str.c_str());
}

TEST_F(SimpleValueTests, test16) {
    JS::RootedObject obj(rt_.getRuntime());
    
    rs::jsapi::Value value(rt_, obj);
    ASSERT_TRUE(value.isObject());
}

TEST_F(SimpleValueTests, test17) {
    rs::jsapi::Value value(rt_, JS_NewObject(rt_, nullptr));
    ASSERT_TRUE(value.isObject());
    ASSERT_STREQ("[object Object]", value.ToString().c_str());
    
    const JS::HandleObject& objRef = value;
    rs::jsapi::Value value2(rt_, objRef);
    ASSERT_TRUE(value2.isObject());
    ASSERT_STREQ("[object Object]", value2.ToString().c_str());
}

TEST_F(SimpleValueTests, test18) {
    JS::RootedObject obj(rt_.getRuntime(), JS_NewArrayObject(rt_, 0));
    
    rs::jsapi::Value value(rt_, obj);
    ASSERT_TRUE(value.isArray());
    ASSERT_STREQ("", value.ToString().c_str());
}

TEST_F(SimpleValueTests, test19) {
    rs::jsapi::Value value(rt_);
    
    JS::RootedObject obj(rt_.getRuntime());
    value.set(obj);
    ASSERT_TRUE(value.isObject());
}

TEST_F(SimpleValueTests, test20) {
    rs::jsapi::Value value(rt_, 42);
    
    JS::RootedObject obj(rt_.getRuntime());
    value.set(obj);
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isNumber());
}

TEST_F(SimpleValueTests, test21) {
    rs::jsapi::Value value(rt_, "hello world");
    
    JS::RootedObject obj(rt_.getRuntime());
    value.set(obj);
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isString());
}

TEST_F(SimpleValueTests, test22) {
    rs::jsapi::Value value(rt_, true);
    
    JS::RootedObject obj(rt_.getRuntime());
    value.set(obj);
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isBoolean());
}

TEST_F(SimpleValueTests, test23) {
    rs::jsapi::Value value(rt_, 3.14159);
    
    JS::RootedObject obj(rt_.getRuntime());
    value.set(obj);
    ASSERT_TRUE(value.isObject());
    ASSERT_FALSE(value.isNumber());
}

TEST_F(SimpleValueTests, test24) {
    rs::jsapi::Value obj(rt_, JS_NewObject(rt_, nullptr));
        
    ASSERT_THROW({
        obj.toNumber();
    }, rs::jsapi::ValueCastException);
    
    ASSERT_THROW({
        obj.toString();
    }, rs::jsapi::ValueCastException);
    
    ASSERT_THROW({
        obj.toBoolean();
    }, rs::jsapi::ValueCastException);
    
    ASSERT_THROW({
        obj.toInt32();
    }, rs::jsapi::ValueCastException);
}

TEST_F(SimpleValueTests, test25) {
    rs::jsapi::Value value(rt_, 42);
        
    ASSERT_THROW({
        value.toObject();
    }, rs::jsapi::ValueCastException);
    
    ASSERT_THROW({
        rs::jsapi::Value result(rt_);
       JS_GetProperty(rt_, value, "test", result);
    }, rs::jsapi::ValueCastException);
}

TEST_F(SimpleValueTests, test26) {
    rs::jsapi::Value obj(rt_, JS_NewObject(rt_, nullptr));
    
    ASSERT_TRUE(obj.isObject());
    ASSERT_TRUE(obj.toObject() != nullptr);    

    JS::RootedValue rv(rt_.getRuntime(), obj);
    ASSERT_TRUE(rv.isObject());
    
    ASSERT_TRUE(obj.isObject());
    ASSERT_TRUE(obj.toObject() != nullptr);    
}

TEST_F(SimpleValueTests, test27) {
    rs::jsapi::Value obj(rt_, JS_NewArrayObject(rt_, 0));
    
    ASSERT_TRUE(obj.isArray());
    ASSERT_TRUE(obj.toObject() != nullptr);    

    JS::RootedValue rv(rt_.getRuntime(), obj);
    ASSERT_TRUE(rv.isObject());
    
    ASSERT_TRUE(obj.isArray());
    ASSERT_TRUE(obj.toObject() != nullptr);    
}

TEST_F(SimpleValueTests, test28) {
    bool thrown = false;
    rs::jsapi::Value obj(rt_, 42);
    
    try {
        obj.toObject();
    } catch (const rs::jsapi::ValueCastException& ex) {
        SanityCheckWhatMessage(ex.what());
        thrown = true;
    }
    
    ASSERT_TRUE(thrown);
}

TEST_F(SimpleValueTests, test29) {
    rs::jsapi::Value func(rt_);
    rt_.Evaluate("(function() { return function() { return 42; } })();", func);
    
    ASSERT_TRUE(func.isFunction());
    
    rs::jsapi::FunctionArguments args(rt_);
    rs::jsapi::Value result(rt_);
    func.CallFunction(args, result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(SimpleValueTests, test30) {
    rs::jsapi::Value func(rt_);
    rt_.Evaluate("(function() { return function(n) { return n; } })();", func);
    
    ASSERT_TRUE(func.isFunction());
    
    rs::jsapi::FunctionArguments args(rt_);
    args.Append(3.14159);
    
    rs::jsapi::Value result(rt_);
    func.CallFunction(args, result);
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_FLOAT_EQ(3.14159, result.toNumber());
}

TEST_F(SimpleValueTests, test31) {
    rs::jsapi::Value value1(rt_);
    ASSERT_TRUE(value1.isUndefined());
    
    rs::jsapi::Value obj(rt_);
    rs::jsapi::Object::Create(rt_, 
        {}, 
        nullptr, 
        nullptr, 
        {},
        nullptr, 
        obj);
    ASSERT_TRUE(!!obj);
    
    value1 = obj;
    ASSERT_TRUE(value1.isObject());
    
    rs::jsapi::Value value2(value1);
    ASSERT_TRUE(value2.isObject());
}

TEST_F(SimpleValueTests, test32) {
    rs::jsapi::Value value1(rt_, 42);
    ASSERT_TRUE(value1.isInt32());    
    ASSERT_EQ(42, value1.toInt32());       
    
    rs::jsapi::Value value2(value1);
    ASSERT_TRUE(value2.isInt32());
    ASSERT_EQ(42, value2.toInt32());
}