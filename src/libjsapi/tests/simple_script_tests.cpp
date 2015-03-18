#include <gtest/gtest.h>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class SimpleScriptTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }            
};

TEST_F(SimpleScriptTests, test1) {
    rs::jsapi::Script script(rt_, "(function(){return 42;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isNumber());
    ASSERT_EQ(42, result().toNumber());
}

TEST_F(SimpleScriptTests, test2) {
    rs::jsapi::Script script(rt_, "(function(){return 42;})();");
    script.Compile();
    
    rs::jsapi::Value result1(rt_);
    script.Execute(result1);

    ASSERT_TRUE(result1().isNumber());
    ASSERT_EQ(42, result1().toNumber());
    
    rs::jsapi::Value result2(rt_);
    script.Execute(result2);

    ASSERT_TRUE(result2().isNumber());
    ASSERT_EQ(42, result2().toNumber());
}

TEST_F(SimpleScriptTests, test3) {
    rs::jsapi::Script bootstrapScript(rt_, "var count = 42;");
    bootstrapScript.Compile();
    bootstrapScript.Execute();
    
    rs::jsapi::Script script(rt_, "(function(){return count++;})();");
    script.Compile();
    
    rs::jsapi::Value result1(rt_);
    script.Execute(result1);

    ASSERT_TRUE(result1().isNumber());
    ASSERT_EQ(42, result1().toNumber());
    
    rs::jsapi::Value result2(rt_);
    script.Execute(result2);

    ASSERT_TRUE(result2().isNumber());
    ASSERT_EQ(43, result2().toNumber());
}

// Test moved
//TEST_F(SimpleScriptTests, test4) {
//}

TEST_F(SimpleScriptTests, test5) {
    rs::jsapi::Script script(rt_, "(function(){return 'hello';})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isString());
    ASSERT_STRCASEEQ("hello", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test6) {
    rs::jsapi::Script script(rt_, "(function(){return 1234;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isNumber());
    ASSERT_FALSE(result().isString());
    ASSERT_STRCASEEQ("1234", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test7) {
    rs::jsapi::Script script(rt_, "(function(){return 1234.567;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isNumber());
    ASSERT_FALSE(result().isString());
    ASSERT_EQ(0, result.ToString().find("1234.567"));
}

TEST_F(SimpleScriptTests, test8) {
    rs::jsapi::Script script(rt_, "(function(){return true;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isBoolean());
    ASSERT_FALSE(result().isString());
    ASSERT_STRCASEEQ("true", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test9) {
    rs::jsapi::Script script(rt_, "(function(){return false;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isBoolean());
    ASSERT_FALSE(result().isString());
    ASSERT_STRCASEEQ("false", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test10) {
    rs::jsapi::Script script(rt_, "(function(){return {};})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isObject());
    ASSERT_FALSE(result().isString());
    ASSERT_STRCASEEQ("[object Object]", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test11) {
    rs::jsapi::Script script(rt_, "(function(){return [];})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isObject());
    ASSERT_FALSE(result().isString());
    ASSERT_STRCASEEQ("[object Object]", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test12) {
    rs::jsapi::Script script(rt_, "(function(){return null;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_TRUE(result().isNull());
    ASSERT_FALSE(result().isString());
    ASSERT_STRCASEEQ("null", result.ToString().c_str());
}