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

    ASSERT_EQ(result().isNumber(), true);
    ASSERT_EQ(result().toNumber(), 42);
}

TEST_F(SimpleScriptTests, test2) {
    rs::jsapi::Script script(rt_, "(function(){return 42;})();");
    script.Compile();
    
    rs::jsapi::Value result1(rt_);
    script.Execute(result1);

    ASSERT_EQ(result1().isNumber(), true);
    ASSERT_EQ(result1().toNumber(), 42);
    
    rs::jsapi::Value result2(rt_);
    script.Execute(result2);

    ASSERT_EQ(result2().isNumber(), true);
    ASSERT_EQ(result2().toNumber(), 42);
}

TEST_F(SimpleScriptTests, test3) {
    rs::jsapi::Script bootstrapScript(rt_, "var count = 42;");
    bootstrapScript.Compile();
    bootstrapScript.Execute();
    
    rs::jsapi::Script script(rt_, "(function(){return count++;})();");
    script.Compile();
    
    rs::jsapi::Value result1(rt_);
    script.Execute(result1);

    ASSERT_EQ(result1().isNumber(), true);
    ASSERT_EQ(result1().toNumber(), 42);
    
    rs::jsapi::Value result2(rt_);
    script.Execute(result2);

    ASSERT_EQ(result2().isNumber(), true);
    ASSERT_EQ(result2().toNumber(), 43);
}

TEST_F(SimpleScriptTests, test4) {
    rs::jsapi::Script bootstrapScript1(rt_, "var count = 42;");
    bootstrapScript1.Compile();
    bootstrapScript1.Execute();
    
    auto context2 = rt_.NewContext();
    rs::jsapi::Script bootstrapScript2(*context2, "var count = 42;");
    bootstrapScript2.Compile();
    bootstrapScript2.Execute();
    
    rs::jsapi::Script script1(rt_, "(function(){return count++;})();");
    script1.Compile();
    
    rs::jsapi::Script script2(*context2, "(function(){return count++;})();");
    script2.Compile();
    
    rs::jsapi::Value result1a(rt_);
    script1.Execute(result1a);

    ASSERT_EQ(result1a().isNumber(), true);
    ASSERT_EQ(result1a().toNumber(), 42);
    
    rs::jsapi::Value result1b(rt_);
    script1.Execute(result1b);

    ASSERT_EQ(result1b().isNumber(), true);
    ASSERT_EQ(result1b().toNumber(), 43);
    
    rs::jsapi::Value result2a(*context2);
    script2.Execute(result2a);

    ASSERT_EQ(result2a().isNumber(), true);
    ASSERT_EQ(result2a().toNumber(), 42);
}

TEST_F(SimpleScriptTests, test5) {
    rs::jsapi::Script script(rt_, "(function(){return 'hello';})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_EQ(result().isString(), true);
    ASSERT_STRCASEEQ(result.ToString().c_str(), "hello");
}

TEST_F(SimpleScriptTests, test6) {
    rs::jsapi::Script script(rt_, "(function(){return 1234;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_EQ(result().isNumber(), true);
    ASSERT_EQ(result().isString(), false);
    ASSERT_STRCASEEQ(result.ToString().c_str(), "1234");
}

TEST_F(SimpleScriptTests, test7) {
    rs::jsapi::Script script(rt_, "(function(){return 1234.567;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_EQ(result().isNumber(), true);
    ASSERT_EQ(result().isString(), false);
    ASSERT_EQ(result.ToString().find("1234.567"), 0);
}

TEST_F(SimpleScriptTests, test8) {
    rs::jsapi::Script script(rt_, "(function(){return true;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_EQ(result().isBoolean(), true);
    ASSERT_EQ(result().isString(), false);
    ASSERT_STRCASEEQ(result.ToString().c_str(), "true");
}

TEST_F(SimpleScriptTests, test9) {
    rs::jsapi::Script script(rt_, "(function(){return false;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_EQ(result().isBoolean(), true);
    ASSERT_EQ(result().isString(), false);
    ASSERT_STRCASEEQ(result.ToString().c_str(), "false");
}

TEST_F(SimpleScriptTests, test10) {
    rs::jsapi::Script script(rt_, "(function(){return {};})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_EQ(result().isObject(), true);
    ASSERT_EQ(result().isString(), false);
    ASSERT_STRCASEEQ(result.ToString().c_str(), "[object Object]");
}

TEST_F(SimpleScriptTests, test11) {
    rs::jsapi::Script script(rt_, "(function(){return [];})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_EQ(result().isObject(), true);
    ASSERT_EQ(result().isString(), false);
    ASSERT_STRCASEEQ(result.ToString().c_str(), "[object Object]");
}

TEST_F(SimpleScriptTests, test12) {
    rs::jsapi::Script script(rt_, "(function(){return null;})();");
    script.Compile();
    
    rs::jsapi::Value result(rt_);
    script.Execute(result);

    ASSERT_EQ(result().isNull(), true);
    ASSERT_EQ(result().isString(), false);
    ASSERT_STRCASEEQ(result.ToString().c_str(), "null");
}