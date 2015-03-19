#include <gtest/gtest.h>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class ScriptExceptionTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }            
};

TEST_F(ScriptExceptionTests, test1) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "!)(*\")(!*\"!");
        script.Compile();
    }, rs::jsapi::ScriptException);        
}

TEST_F(ScriptExceptionTests, test1b) {        
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
    ASSERT_TRUE(result().isNumber());
    ASSERT_EQ(42, result().toNumber());
}

TEST_F(ScriptExceptionTests, test4b) {        
    ASSERT_THROW({                
        rt_.Evaluate("!)(*\")(!*\"!");
    }, rs::jsapi::ScriptException);        
    
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return 42;})()", result);        
    ASSERT_EQ(result().isNumber(), true);
    ASSERT_EQ(42, result().toNumber());
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
    ASSERT_EQ(result().isNumber(), true);
    ASSERT_EQ(42, result().toNumber());
}

TEST_F(ScriptExceptionTests, test5b) {        
    ASSERT_THROW({                
        rt_.Evaluate("var xyz = abc;");
    }, rs::jsapi::ScriptException);  
    
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return 42;})()", result);        
    ASSERT_EQ(result().isNumber(), true);
    ASSERT_EQ(42, result().toNumber());
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
    ASSERT_TRUE(result().isNumber());
    ASSERT_EQ(42, result().toNumber());
}

TEST_F(ScriptExceptionTests, test6b) {        
    ASSERT_THROW({                
        rs::jsapi::Value result(rt_);
        rt_.Evaluate("(function(){return abc;})()", result);
    }, rs::jsapi::ScriptException);      
    
    rs::jsapi::Value result(rt_);
    rt_.Evaluate("(function(){return 42;})()", result);
    ASSERT_TRUE(result().isNumber());
    ASSERT_EQ(42, result().toNumber());
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