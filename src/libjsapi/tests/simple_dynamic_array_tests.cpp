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

rs::jsapi::Runtime rt_;

class SimpleDynamicArrayTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }            
};

class SimpleDynamicArrayTestException : public std::exception {
public:
    SimpleDynamicArrayTestException(const char* msg) : msg_(msg) {
    }
    
    const char* what() const throw() override { return msg_.c_str(); }
    
private:
    const std::string msg_;
};

TEST_F(SimpleDynamicArrayTests, test1) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        [](int index, rs::jsapi::Value& value) { value = 42; },
        nullptr,
        []() { return 1; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    rs::jsapi::Value value(*context);
    ASSERT_TRUE(JS_GetElement(*context, array, 0, value));
    ASSERT_EQ(42, value.toInt32());
    
    uint32_t length = -1;
    ASSERT_TRUE(JS_GetArrayLength(*context, array, &length));
    ASSERT_EQ(1, length);    
}

TEST_F(SimpleDynamicArrayTests, test2) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        [](int index, rs::jsapi::Value& value) { value = index; },
        nullptr,
        []() { return 10; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    uint32_t length = -1;
    ASSERT_TRUE(JS_GetArrayLength(*context, array, &length));
    ASSERT_EQ(10, length);
    
    for (int i = 0; i < 10; ++i) {
        rs::jsapi::Value value(*context);
        ASSERT_TRUE(JS_GetElement(*context, array, i, value));
        ASSERT_EQ(i, value.toInt32());
    }
}

TEST_F(SimpleDynamicArrayTests, test3) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        [](int index, rs::jsapi::Value& value) { value = index; },
        nullptr,
        []() { return 10; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    rt_.Evaluate("var myfunc = function(arr, n) { return arr[n]; };");
   
    for (int i = 0; i < 10; ++i) {
        rs::jsapi::FunctionArguments args(*context);
        args.Append(array);
        args.Append(i);
        
        rs::jsapi::Value result(*context);
        rt_.Call("myfunc", args, result);
        
        ASSERT_TRUE(result.isInt32());
        ASSERT_EQ(i, result.toInt32());
    }
}

TEST_F(SimpleDynamicArrayTests, test4) {
    auto context = rt_.NewContext();    
    rs::jsapi::Value array(*context);
    
    context->Evaluate("var myfunc = function(arr) { return arr.length; };");
    
    for (int i = 0; i < 100; ++i) {
        rs::jsapi::DynamicArray::Create(*context,
            [](int index, rs::jsapi::Value& value) { value = index; },
            nullptr,
            [&]() { return i; },
            nullptr,
            array);
        ASSERT_TRUE(!!array);

        rs::jsapi::FunctionArguments args(*context);
        args.Append(array);
        
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);

        ASSERT_TRUE(result.isInt32());
        ASSERT_EQ(i, result.toInt32());
    }
}

TEST_F(SimpleDynamicArrayTests, test5) {    
    auto count = 0;
    
    if (true) {
        auto context = rt_.NewContext();
        
        rs::jsapi::Value obj(*context);
        rs::jsapi::DynamicArray::Create(
            *context, 
            nullptr, 
            nullptr, 
            nullptr,
            [&]() { ++count; },
            obj);
        ASSERT_TRUE(!!obj);
    }
    
    ASSERT_EQ(1, count);
}

TEST_F(SimpleDynamicArrayTests, test6) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        [](int index, rs::jsapi::Value& value) { value = index; },
        nullptr,
        []() { return 10; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    rt_.Evaluate("var myfunc = function(arr, n) { return arr[n]; };");
   
    for (int i = 0; i < 10; ++i) {
        rs::jsapi::FunctionArguments args(*context);
        args.Append(array);
        args.Append(std::to_string(i));
        
        rs::jsapi::Value result(*context);
        rt_.Call("myfunc", args, result);
        
        ASSERT_TRUE(result.isInt32());
        ASSERT_EQ(i, result.toInt32());
    }
}

TEST_F(SimpleDynamicArrayTests, test7) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        [](int index, rs::jsapi::Value& value) { value = index; },
        nullptr,
        []() { return 10; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    rt_.Evaluate("var myfunc = function(arr, n) { return arr[n]; };");
   
    for (int i = 0; i < 10; ++i) {
        rs::jsapi::FunctionArguments args(*context);
        args.Append(array);
        args.Append("abc");
        
        rs::jsapi::Value result(*context);
        rt_.Call("myfunc", args, result);
        
        ASSERT_TRUE(result.isUndefined());
    }
}

TEST_F(SimpleDynamicArrayTests, test8) {
    auto context = rt_.NewContext();

    rs::jsapi::Value array(*context);
    rs::jsapi::DynamicArray::Create(*context, 
        nullptr, 
        nullptr, 
        nullptr,
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    rs::jsapi::DynamicArray::SetPrivate(array, 123456789, this);
    
    uint64_t data = 0;
    void* that = nullptr;
    ASSERT_TRUE(rs::jsapi::DynamicArray::GetPrivate(array, data, that));
    ASSERT_EQ(123456789, data);
    ASSERT_EQ(this, that);
}

TEST_F(SimpleDynamicArrayTests, test9) {
    auto context = rt_.NewContext();

    rs::jsapi::Value array(*context);
    ASSERT_FALSE(rs::jsapi::DynamicArray::SetPrivate(array, 123456789, this));
    
    uint64_t data = 0;
    void* that = nullptr;
    ASSERT_FALSE(rs::jsapi::DynamicArray::GetPrivate(array, data, that));
    ASSERT_NE(123456789, data);
    ASSERT_NE(this, that);
}

TEST_F(SimpleDynamicArrayTests, test10) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        [](int index, rs::jsapi::Value& value) { throw SimpleDynamicArrayTestException("It happened!"); },
        nullptr,
        []() { return 1; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    context->Evaluate("var myfunc=function(n, i){ return n[i];};");

    rs::jsapi::FunctionArguments args(*context);
    args.Append(array);
    args.Append(0);
    
    ASSERT_THROW({
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);
    }, rs::jsapi::ScriptException);           
}

TEST_F(SimpleDynamicArrayTests, test11) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        [](int index, rs::jsapi::Value& value) { throw SimpleDynamicArrayTestException("It happened!"); },
        nullptr,
        []() { return 1; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    context->Evaluate("var myfunc=function(n, i){ return n[i];};");

    rs::jsapi::FunctionArguments args(*context);
    args.Append(array);
    args.Append(0);
    
    bool thrown = false;
    try {
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);
    } catch (const rs::jsapi::ScriptException& ex) {
        thrown = true;
    }
    
    ASSERT_TRUE(thrown);
}

TEST_F(SimpleDynamicArrayTests, test12) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        nullptr,
        [](int index, const rs::jsapi::Value& value) { throw SimpleDynamicArrayTestException("It happened!"); },        
        []() { return 1; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    context->Evaluate("var myfunc=function(n, i){ n[i] = true;};");

    rs::jsapi::FunctionArguments args(*context);
    args.Append(array);
    args.Append(0);
    
    ASSERT_THROW({
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);
    }, rs::jsapi::ScriptException);           
}

TEST_F(SimpleDynamicArrayTests, test13) {
    auto context = rt_.NewContext();
    
    rs::jsapi::Value array(*context);    
    rs::jsapi::DynamicArray::Create(*context,
        nullptr,
        [](int index, const rs::jsapi::Value& value) { throw SimpleDynamicArrayTestException("It happened!"); },        
        []() { return 1; },
        nullptr,
        array);
    ASSERT_TRUE(!!array);
    
    context->Evaluate("var myfunc=function(n, i){ n[i] = true;};");

    rs::jsapi::FunctionArguments args(*context);
    args.Append(array);
    args.Append(0);
    
    bool thrown = false;
    try {
        rs::jsapi::Value result(*context);
        context->Call("myfunc", args, result);
    } catch (const rs::jsapi::ScriptException& ex) {
        thrown = true;
    }
    
    ASSERT_TRUE(thrown);        
}