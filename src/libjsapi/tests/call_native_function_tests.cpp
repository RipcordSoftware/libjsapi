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

class CallNativeFunctionTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }
    
    static void Echo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
};

class CallNativeFunctionTestException : public std::exception {
public:
    CallNativeFunctionTestException(const char* msg) : msg_(msg) {
    }
    
    const char* what() const throw() override { return msg_.c_str(); }
    
private:
    const std::string msg_;
};

void CallNativeFunctionTests::Echo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (args.size() > 0) {
        result = args[0];
    } else {    
        result = JS::NullHandleValue;
    }
}

TEST_F(CallNativeFunctionTests, test1) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "getTheAnswer", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { result = 42; });
    
    rs::jsapi::Value result(cx);
    cx.Evaluate("(function(){return getTheAnswer();})();", result);
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());    
}

TEST_F(CallNativeFunctionTests, test1b) {
    rs::jsapi::Context cx;
    int valueClosure = 42;
    
    rs::jsapi::Global::DefineFunction(cx, "getTheAnswer", [&](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { result = valueClosure++; });
    
    rs::jsapi::Value result(cx);
    cx.Call("getTheAnswer", result);    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());    
    
    cx.Call("getTheAnswer", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(43, result.toInt32());    
    
    cx.Call("getTheAnswer", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(44, result.toInt32());
}

TEST_F(CallNativeFunctionTests, test2) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "getLorem", 
        [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { result = "Lorem ipsum"; });
    
    rs::jsapi::Value result(cx);
    cx.Evaluate("(function(){return getLorem();})();", result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("Lorem ipsum", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test3) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::FunctionArguments args(cx);
    args.Append(true);
    
    rs::jsapi::Value result(cx);
    cx.Call("echo", args, result); 
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_TRUE(result.toBoolean());
}

TEST_F(CallNativeFunctionTests, test4) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::FunctionArguments args(cx);
    args.Append(42);
    
    rs::jsapi::Value result(cx);
    cx.Call("echo", args, result); 
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(CallNativeFunctionTests, test5) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::FunctionArguments args(cx);
    args.Append("hello");
    
    rs::jsapi::Value result(cx);
    cx.Call("echo", args, result); 
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test6) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::Value value(cx, "hello");
    rs::jsapi::FunctionArguments args(cx);
    args.Append(value);
    
    rs::jsapi::Value result(cx);
    cx.Call("echo", args, result);     
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello", result.ToString().c_str());
}

TEST_F(CallNativeFunctionTests, test7) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::Value value(cx, true);
    rs::jsapi::FunctionArguments args(cx);
    args.Append(value);
    
    rs::jsapi::Value result(cx);
    cx.Call("echo", args, result);     
    
    ASSERT_TRUE(result.isBoolean());
    ASSERT_TRUE(result.toBoolean());
}

TEST_F(CallNativeFunctionTests, test8) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::Value value(cx, 42);
    rs::jsapi::FunctionArguments args(cx);
    args.Append(value);
    
    rs::jsapi::Value result(cx);
    cx.Call("echo", args, result);     
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_TRUE(result.toInt32());
}

TEST_F(CallNativeFunctionTests, test9) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "echo", &CallNativeFunctionTests::Echo);
    
    rs::jsapi::Value value(cx, 3.14159);
    rs::jsapi::FunctionArguments args(cx);
    args.Append(value);
    
    rs::jsapi::Value result(cx);
    cx.Call("echo", args, result);     
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_FLOAT_EQ(3.14159, result.toNumber());
}

TEST_F(CallNativeFunctionTests, test10) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "makeMeSad", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
        throw CallNativeFunctionTestException("It happened!");
    });
    
    ASSERT_THROW({
        rs::jsapi::Value result(cx);
        cx.Evaluate("(function(){return makeMeSad();})();", result);
    }, rs::jsapi::ScriptException);
}

TEST_F(CallNativeFunctionTests, test11) {
    rs::jsapi::Context cx;
    
    rs::jsapi::Global::DefineFunction(cx, "makeMeSad", [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
        throw CallNativeFunctionTestException("It happened!");
    });
    
    rs::jsapi::Value result(cx);
    cx.Evaluate("(function(){ try { return makeMeSad(); } catch (e) { return e.message; } })();", result);
    
    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("It happened!", result.ToString().c_str());
}