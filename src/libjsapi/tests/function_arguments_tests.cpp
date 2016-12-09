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

class FunctionArgumentsTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
};

TEST_F(FunctionArgumentsTests, test1) {
    rs::jsapi::Context cx;
    rs::jsapi::FunctionArguments args(cx);
    
    ASSERT_EQ(0, args.getLength());
    args.Clear();
    ASSERT_EQ(0, args.getLength());
    ASSERT_TRUE(args.Empty());
}

TEST_F(FunctionArgumentsTests, test2) {
    rs::jsapi::Context cx;
    rs::jsapi::FunctionArguments args(cx);   
    
    args.Append(99);
 
    ASSERT_EQ(1, args.getLength());
    ASSERT_FALSE(args.Empty());
    args.Clear();
    ASSERT_TRUE(args.Empty());
}

TEST_F(FunctionArgumentsTests, test3) {
    rs::jsapi::Context cx;
    rs::jsapi::FunctionArguments args(cx);   
    
    args.Append(99);
 
    ASSERT_EQ(1, args.getLength());
    ASSERT_FALSE(args.Empty());
    ASSERT_TRUE(args[0].isInt32());
    ASSERT_EQ(99, args[0].toInt32());
}

TEST_F(FunctionArgumentsTests, test4) {
    rs::jsapi::Context cx;
    rs::jsapi::FunctionArguments args(cx);   
    
    args.Append(99);
    ASSERT_EQ(1, args.getLength());
    ASSERT_FALSE(args.Empty());

    args.Append("Hello world");
    ASSERT_EQ(2, args.getLength());
    ASSERT_FALSE(args.Empty());
    
    ASSERT_TRUE(args[0].isInt32());
    ASSERT_EQ(99, args[0].toInt32());    
    
    ASSERT_TRUE(args[1].isString());
    rs::jsapi::Value result(cx, args[1].toString());    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("Hello world", result.ToString().c_str());
    
    args.Clear();
    ASSERT_TRUE(args.Empty());
}

TEST_F(FunctionArgumentsTests, test5) {
    rs::jsapi::Context cx;
    rs::jsapi::FunctionArguments args(cx);   
    
    JS::RootedObject obj(cx.getContext());
    args.Append(obj);
    ASSERT_EQ(1, args.getLength());
    ASSERT_FALSE(args.Empty());
    
    JS::RootedValue value(cx.getContext());
    args.Append(value);
    ASSERT_EQ(2, args.getLength());
    ASSERT_FALSE(args.Empty());
}

TEST_F(FunctionArgumentsTests, test6) {
    rs::jsapi::Context cx;
    rs::jsapi::FunctionArguments args(cx);
    
    ASSERT_THROW({        
        args[0].isString();
    }, rs::jsapi::FunctionArgumentsIndexException);
    
    bool thrown = false;
    try {
        args[0].isString();
    } catch (const rs::jsapi::FunctionArgumentsIndexException& ex) {
        ASSERT_NE(nullptr, ex.what());
        thrown = true;
    }
    
    args.Append(true);
    ASSERT_EQ(1, args.getLength());
    ASSERT_TRUE(args[0].isBoolean());
    ASSERT_THROW({ args[1].isBoolean(); }, rs::jsapi::FunctionArgumentsIndexException);
}