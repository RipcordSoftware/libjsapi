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
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(SimpleScriptTests, test2) {
    rs::jsapi::Script script(rt_, "(function(){return 42;})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result1(rt_);
    ASSERT_TRUE(script.Execute(result1));

    ASSERT_TRUE(result1.isNumber());
    ASSERT_EQ(42, result1.toNumber());
    
    rs::jsapi::Value result2(rt_);
    ASSERT_TRUE(script.Execute(result2));

    ASSERT_TRUE(result2.isNumber());
    ASSERT_EQ(42, result2.toNumber());
}

TEST_F(SimpleScriptTests, test3) {
    rs::jsapi::Script bootstrapScript(rt_, "var count = 42;");
    bootstrapScript.Compile();
    ASSERT_TRUE(bootstrapScript.Execute());
    
    rs::jsapi::Script script(rt_, "(function(){return count++;})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result1(rt_);
    ASSERT_TRUE(script.Execute(result1));

    ASSERT_TRUE(result1.isNumber());
    ASSERT_EQ(42, result1.toNumber());
    
    rs::jsapi::Value result2(rt_);
    ASSERT_TRUE(script.Execute(result2));

    ASSERT_TRUE(result2.isNumber());
    ASSERT_EQ(43, result2.toNumber());
}

// Test moved
//TEST_F(SimpleScriptTests, test4) {
//}

TEST_F(SimpleScriptTests, test5) {
    rs::jsapi::Script script(rt_, "(function(){return 'hello';})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isString());
    ASSERT_STREQ("hello", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test6) {
    rs::jsapi::Script script(rt_, "(function(){return 1234;})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isNumber());
    ASSERT_FALSE(result.isString());
    ASSERT_STREQ("1234", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test7) {
    rs::jsapi::Script script(rt_, "(function(){return 1234.567;})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isNumber());
    ASSERT_FALSE(result.isString());
    ASSERT_EQ(0, result.ToString().find("1234.567"));
}

TEST_F(SimpleScriptTests, test7b) {
    rs::jsapi::Script script(rt_, "(function(){return 1234.567;})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isNumber());
    ASSERT_FLOAT_EQ(1234.567, result.toNumber());
}

TEST_F(SimpleScriptTests, test8) {
    rs::jsapi::Script script(rt_, "(function(){return true;})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isBoolean());
    ASSERT_FALSE(result.isString());
    ASSERT_STREQ("true", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test9) {
    rs::jsapi::Script script(rt_, "(function(){return false;})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isBoolean());
    ASSERT_FALSE(result.isString());
    ASSERT_STREQ("false", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test10) {
    rs::jsapi::Script script(rt_, "(function(){return {};})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isObject());
    ASSERT_FALSE(result.isString());
    ASSERT_STREQ("[object Object]", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test11) {
    rs::jsapi::Script script(rt_, "(function(){return [];})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isObject());
    ASSERT_FALSE(result.isString());
    ASSERT_STREQ("", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test11b) {
    rs::jsapi::Script script(rt_, "(function(){return [1,2,3];})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isObject());
    ASSERT_FALSE(result.isString());
    ASSERT_STREQ("1,2,3", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test12) {
    rs::jsapi::Script script(rt_, "(function(){return null;})();");
    ASSERT_TRUE(script.Compile());
    
    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));

    ASSERT_TRUE(result.isNull());
    ASSERT_FALSE(result.isString());
    ASSERT_STREQ("null", result.ToString().c_str());
}

TEST_F(SimpleScriptTests, test13) {
    rs::jsapi::Script script(rt_, "(function(){return 42;})();");

    rs::jsapi::Value result(rt_);
    ASSERT_TRUE(script.Execute(result));
    
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(42, result.toInt32());
}

TEST_F(SimpleScriptTests, test14) {
    rs::jsapi::Script script(rt_, "(function(){})();");

    ASSERT_TRUE(script.Execute());
}