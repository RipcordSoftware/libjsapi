#include <gtest/gtest.h>

#include <thread>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class MultiContextTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }            
};

TEST_F(MultiContextTests, test1) {
    auto context = rt_.NewContext();
    rs::jsapi::Script script(*context, "(function(){return 42;})();");
    script.Compile();
    
    rs::jsapi::Value result(*context);
    script.Execute(result);
    
    ASSERT_TRUE(result.isNumber());
    ASSERT_EQ(42, result.toNumber());
}

TEST_F(MultiContextTests, test2) {
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

    ASSERT_TRUE(result1a.isNumber());
    ASSERT_EQ(42, result1a.toNumber());
    
    rs::jsapi::Value result1b(rt_);
    script1.Execute(result1b);

    ASSERT_TRUE(result1b.isNumber());
    ASSERT_EQ(43, result1b.toNumber());
    
    rs::jsapi::Value result2a(*context2);
    script2.Execute(result2a);

    ASSERT_TRUE(result2a.isNumber());
    ASSERT_EQ(42, result2a.toNumber());
}

TEST_F(MultiContextTests, test3) {        
    std::thread t([]() {  
        ASSERT_THROW({
            rt_.NewContext();
        }, rs::jsapi::RuntimeWrongThreadException);
    });
       
    t.join();
}

TEST_F(MultiContextTests, test4) {        
    std::thread t([]() {  
        ASSERT_THROW({
            rs::jsapi::Context& cx = rt_;
        }, rs::jsapi::RuntimeWrongThreadException);
    });
       
    t.join();
}

TEST_F(MultiContextTests, test5) {        
    std::thread t([]() {  
        ASSERT_THROW({
            rt_.getRuntime();
        }, rs::jsapi::RuntimeWrongThreadException);
    });
       
    t.join();
}

TEST_F(MultiContextTests, test6) {        
    auto cx = rt_.NewContext();
    
    std::thread t([&]() {  
        rs::jsapi::Script script(*cx, "(function(){return 42;})();");
        script.Compile();

        rs::jsapi::Value result(*cx);
        script.Execute(result);

        ASSERT_TRUE(result.isNumber());
        ASSERT_EQ(42, result.toNumber());
    });
       
    t.join();
}