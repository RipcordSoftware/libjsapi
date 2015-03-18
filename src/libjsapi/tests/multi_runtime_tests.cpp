#include <gtest/gtest.h>

#include <thread>

#include "../libjsapi.h"

static rs::jsapi::Runtime rt_;

class MultiRuntimeTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }                    
};

TEST_F(MultiRuntimeTests, test1) {
    ASSERT_THROW({
        rs::jsapi::Runtime rt;
    }, rs::jsapi::RuntimeThreadInstanceException);
}

TEST_F(MultiRuntimeTests, test2) {
    std::thread t([](){
        rs::jsapi::Runtime rt;
        rs::jsapi::Script script(rt, "(function(){return 42;})();");
        script.Compile();

        rs::jsapi::Value result(rt);
        script.Execute(result);

        ASSERT_EQ(result().isNumber(), true);
        ASSERT_EQ(result().toNumber(), 42);
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test3) {
    std::thread t([](){
        rs::jsapi::Runtime rt1;
    
        ASSERT_THROW({
            rs::jsapi::Runtime rt2;
        }, rs::jsapi::RuntimeThreadInstanceException);
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test4) {
    std::thread t([](){
        rs::jsapi::Runtime rt1;
    
        ASSERT_THROW({
            rs::jsapi::Runtime rt2;
        }, rs::jsapi::RuntimeThreadInstanceException);
        
        ASSERT_THROW({
            rs::jsapi::Runtime rt3;
        }, rs::jsapi::RuntimeThreadInstanceException);
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test5) {
    std::thread t([](){
        do {
            rs::jsapi::Runtime rt;
            rs::jsapi::Script script(rt, "(function(){return 42;})();");
            script.Compile();

            rs::jsapi::Value result(rt);
            script.Execute(result);

            ASSERT_EQ(result().isNumber(), true);
            ASSERT_EQ(result().toNumber(), 42);
        } while (false);
        
        do {
            rs::jsapi::Runtime rt;
            rs::jsapi::Script script(rt, "(function(){return 42;})();");
            script.Compile();

            rs::jsapi::Value result(rt);
            script.Execute(result);

            ASSERT_EQ(result().isNumber(), true);
            ASSERT_EQ(result().toNumber(), 42);
        } while (false);
    });
    
    t.join();
}

TEST_F(MultiRuntimeTests, test6) {
    std::thread t([](){
        rs::jsapi::Runtime rt;
        
        rs::jsapi::Script bootstrapScript1(rt, "var count = 42;");
        bootstrapScript1.Compile();
        bootstrapScript1.Execute();

        auto context2 = rt.NewContext();
        rs::jsapi::Script bootstrapScript2(*context2, "var count = 42;");
        bootstrapScript2.Compile();
        bootstrapScript2.Execute();

        rs::jsapi::Script script1(rt, "(function(){return count++;})();");
        script1.Compile();

        rs::jsapi::Script script2(*context2, "(function(){return count++;})();");
        script2.Compile();

        rs::jsapi::Value result1a(rt);
        script1.Execute(result1a);

        ASSERT_EQ(result1a().isNumber(), true);
        ASSERT_EQ(result1a().toNumber(), 42);

        rs::jsapi::Value result1b(rt);
        script1.Execute(result1b);

        ASSERT_EQ(result1b().isNumber(), true);
        ASSERT_EQ(result1b().toNumber(), 43);

        rs::jsapi::Value result2a(*context2);
        script2.Execute(result2a);

        ASSERT_EQ(result2a().isNumber(), true);
        ASSERT_EQ(result2a().toNumber(), 42);
    });
    
    t.join();
}