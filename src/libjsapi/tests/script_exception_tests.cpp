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

class ScriptExceptionTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }          

    rs::jsapi::Runtime rt_;
};

TEST_F(ScriptExceptionTests, test1) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "!)(*\")(!*\"!");
        script.Compile();
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test1b) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "!)(*\")(!*\"!");
        script.Execute();
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test1c) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "!)(*\")(!*\"!");
        
        rs::jsapi::Value result(rt_);
        script.Execute(result);
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test1d) {        
    ASSERT_THROW({                
        rt_.Evaluate("!)(*\")(!*\"!");
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test2) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "var xyz = abc;");
        script.Compile();
        script.Execute();
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test2b) {        
    ASSERT_THROW({                
        rt_.Evaluate("var xyz = abc;");
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test3) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "(function(){return abc;})()");
        script.Compile();
        rs::jsapi::Value result(rt_);
        script.Execute(result);
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test3b) {        
    ASSERT_THROW({                
        rt_.Evaluate("(function(){return abc;})()");
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test4) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "!)(*\")(!*\"!");
        script.Compile();
    }, rs::jsapi::ScriptException);        
    
    rs::jsapi::Script script(rt_, "(function(){return 42;})()");
    script.Compile();
    rs::jsapi::Value result(rt_);
    script.Execute(result);
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test4b) {        
    ASSERT_THROW({                
        rt_.Evaluate("!)(*\")(!*\"!");
    }, rs::jsapi::ScriptException);        
    
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return 42;})()", result);        
    ASSERT_EQ(result.isNumber(), true);
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test5) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "var xyz = abc;");
        script.Compile();
        script.Execute();
    }, rs::jsapi::ScriptException);  
    
    rs::jsapi::Script script(rt_, "(function(){return 42;})()");
    script.Compile();
    rs::jsapi::Value result(rt_);
    script.Execute(result);
    ASSERT_EQ(result.isNumber(), true);
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test5b) {        
    ASSERT_THROW({                
        rt_.Evaluate("var xyz = abc;");
    }, rs::jsapi::ScriptException);  
    
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return 42;})()", result);        
    ASSERT_EQ(result.isNumber(), true);
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test6) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "(function(){return abc;})()");
        script.Compile();
        rs::jsapi::Value result(rt_);
        script.Execute(result);
    }, rs::jsapi::ScriptException);      
    
    rs::jsapi::Script script(rt_, "(function(){return 42;})()");
    script.Compile();
    rs::jsapi::Value result(rt_);
    script.Execute(result);
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test6b) {        
    ASSERT_THROW({                
        rs::jsapi::Value result(rt_);
        rt_.Evaluate("(function(){return abc;})()", result);
    }, rs::jsapi::ScriptException);      
    
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return 42;})()", result);
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test7) {
    std::string message;
    unsigned lineno = 0;
    unsigned column = 0;

    try {
        rs::jsapi::Script script(rt_, "var xyz = abc;");
        script.Compile();
        script.Execute();   
    } catch (const rs::jsapi::ScriptException& e) {
        message = e.what();
        lineno = e.lineno;
        column = e.column;
    }

    ASSERT_STRCASEEQ("ReferenceError: abc is not defined", message.c_str());
    ASSERT_EQ(1, lineno);
    ASSERT_EQ(8, column);
}

TEST_F(ScriptExceptionTests, test7b) {
    std::string message;
    unsigned lineno = 0;
    unsigned column = 0;

    try {
        rt_.Evaluate("var xyz = abc;");
    } catch (const rs::jsapi::ScriptException& e) {
        message = e.what();
        lineno = e.lineno;
        column = e.column;
    }

    ASSERT_STRCASEEQ("ReferenceError: abc is not defined", message.c_str());
    ASSERT_EQ(1, lineno);
    ASSERT_EQ(8, column);
}

TEST_F(ScriptExceptionTests, test8) {
    ASSERT_THROW({                        
        rt_.Call("yoyoyoyo");
    }, rs::jsapi::ScriptException);         
}

TEST_F(ScriptExceptionTests, test9) {
    ASSERT_THROW({
        rs::jsapi::Value result(rt_);
        rt_.Call("yoyoyoyo", result);
    }, rs::jsapi::ScriptException);         
}

TEST_F(ScriptExceptionTests, test10) {
    auto script = R"((function(){ return function() { abc(); } })();)";
    rs::jsapi::Value func(rt_);
    rt_.Evaluate(script, func);
    
    ASSERT_THROW({
        rs::jsapi::FunctionArguments args{rt_};
        func.CallFunction(args);
    }, rs::jsapi::ScriptException);
    
    // check the previous exception has been cleared
    script = R"((function(){ return 42; })();)";
    rs::jsapi::Value result(rt_);
    rt_.Evaluate(script, result);
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test11) {
    auto script = R"((function(){ return function() { abc(); } })();)";
    rs::jsapi::Value func(rt_);    
    rt_.Evaluate(script, func);
    
    ASSERT_THROW({
        rs::jsapi::FunctionArguments args{rt_};
        rs::jsapi::Value result(rt_);
        func.CallFunction(args, result);
    }, rs::jsapi::ScriptException);
    
    // check the previous exception has been cleared
    script = R"((function(){ return 42; })();)";
    rs::jsapi::Value result(rt_);
    rt_.Evaluate(script, result);
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test12) {
    auto script = R"((function(){ return function() { abc(); } })();)";
    rs::jsapi::Value func(rt_);
    rt_.Evaluate(script, func);
    
    rs::jsapi::FunctionArguments args{rt_};
    func.CallFunction(args, false);
    
    // check the previous exception has been cleared
    script = R"((function(){ return 42; })();)";
    rs::jsapi::Value result(rt_);
    rt_.Evaluate(script, result);
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(ScriptExceptionTests, test13) {
    auto script = R"((function(){ return function() { abc(); } })();)";
    rs::jsapi::Value func(rt_);
    rt_.Evaluate(script, func);
    
    rs::jsapi::FunctionArguments args{rt_};
    rs::jsapi::Value result(rt_);
    func.CallFunction(args, result, false);
    
    // check the previous exception has been cleared
    script = R"((function(){ return 42; })();)";
    rt_.Evaluate(script, result);
    ASSERT_EQ(42, result.toNumber());
}