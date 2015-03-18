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