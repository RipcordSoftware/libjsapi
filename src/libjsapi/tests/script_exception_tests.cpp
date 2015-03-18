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

TEST_F(ScriptExceptionTests, test2) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "var xyz = abc;");
        script.Compile();
        script.Execute();
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

TEST_F(ScriptExceptionTests, test4) {        
    ASSERT_THROW({                
        rs::jsapi::Script script(rt_, "!)(*\")(!*\"!");
        script.Compile();
    }, rs::jsapi::ScriptException);        
    
    rs::jsapi::Script script(rt_, "(function(){return 42;})()");
    script.Compile();
    rs::jsapi::Value result(rt_);
    script.Execute(result);
    ASSERT_EQ(result().isNumber(), true);
    ASSERT_EQ(result().toNumber(), 42);
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
    ASSERT_EQ(result().toNumber(), 42);
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
    ASSERT_EQ(result().isNumber(), true);
    ASSERT_EQ(result().toNumber(), 42);
}