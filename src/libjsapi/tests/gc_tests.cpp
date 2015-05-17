#include <gtest/gtest.h>

#include "../libjsapi.h"

rs::jsapi::Runtime rt_;

class GCTests : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }
    
    virtual void TearDown() {
        
    }    
};

TEST_F(GCTests, test1) {
    auto cx = rt_.NewContext();
    
    rs::jsapi::Value value(*cx, 10);
    rs::jsapi::Global::DefineProperty(*cx, "test", value);
    
    cx->MaybeGC();
    
    rs::jsapi::Value result(*cx);
    cx->Evaluate("(function(){return test;})();", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(10, result.toInt32());
    
    rt_.GCNow();
    cx->Evaluate("(function(){return test;})();", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(10, result.toInt32());
}

TEST_F(GCTests, test2) {
    auto cx = rt_.NewContext();
    
    if (true) {
        rs::jsapi::Value value(*cx, 10);
        rs::jsapi::Global::DefineProperty(*cx, "test", value);
    }    
    
    rt_.GCNow();
    
    rs::jsapi::Value result(*cx);
    cx->Evaluate("(function(){return test;})();", result);
    ASSERT_TRUE(result.isInt32());
    ASSERT_EQ(10, result.toInt32());
}

TEST_F(GCTests, test3) {
    auto cx = rt_.NewContext();
    
    bool deleted = false;
    
    if (true) {
        rs::jsapi::Value obj(*cx);
        rs::jsapi::Object::Create(*cx, {}, nullptr, nullptr, {},
            [&]() { deleted = true; }, obj);

        ASSERT_TRUE(rs::jsapi::Global::DefineProperty(*cx, "test", obj));
        
        rs::jsapi::Value result(*cx);
        cx->Evaluate("(function(){return test;})();", result);
        ASSERT_TRUE(result.isObject());
    }
    
    ASSERT_TRUE(rs::jsapi::Global::DeleteProperty(*cx, "test"));
    
    rt_.GCNow();
    ASSERT_TRUE(deleted);
}

TEST_F(GCTests, test4) {
    auto cx = rt_.NewContext();
    
    bool deleted = false;
    
    ASSERT_TRUE(rs::jsapi::Global::DefineFunction(*cx, "testFn",
        [](const std::vector<rs::jsapi::Value>&, rs::jsapi::Value&) {}));
    
    if (true) {                
        rs::jsapi::Value func(*cx);
        cx->Evaluate("(function(){return testFn;})();", func);
        ASSERT_TRUE(func.isFunction());
        
        // add a child object to the function so we can track the delete
        rs::jsapi::Value obj(*cx);
        rs::jsapi::Object::Create(*cx, {}, nullptr, nullptr, {},
            [&]() { deleted = true; }, obj);
            
        ASSERT_TRUE(JS_DefineProperty(*cx, func, "test", obj.getHandleObject(), 0));
    }
    
    ASSERT_TRUE(rs::jsapi::Global::DeleteFunction(*cx, "testFn"));
    
    ASSERT_THROW({
        rs::jsapi::Value result(*cx);
        cx->Evaluate("(function(){return testFn;})();", result);
    }, rs::jsapi::ScriptException);
    
    // force a GC pass and then check the delete flag
    rt_.GCNow();
    ASSERT_TRUE(deleted);
}

#if 0
TEST_F(GCTests, test5) {
    auto cx = rt_.NewContext();    
    
    bool deleted = false;
        
    rs::jsapi::Value obj(*cx);
    rs::jsapi::Object::Create(*cx, {}, nullptr, nullptr, {},
        [&]() { deleted = true; }, obj);
        
    // force a GC pass and then check the delete flag
    rt_.GCNow();
    ASSERT_FALSE(deleted);       
}
#endif

#if 0
int deleted = 0;
static void Finalize(JSFreeOp* fop, JSObject* obj) {
    ++deleted;
}

JSClass klass = { 
    "rs_jsapi_test_object", 0, JS_PropertyStub, JS_DeletePropertyStub,
    JS_PropertyStub, JS_StrictPropertyStub, JS_EnumerateStub, JS_ResolveStub, 
    JS_ConvertStub, &Finalize
};

void doIt(JSContext* cx) {
    JS::RootedObject obj(cx, JS_NewObject(cx, &klass, JS::NullPtr(), JS::NullPtr()));
    JS_NewObject(cx, &klass, JS::NullPtr(), JS::NullPtr());

    // force a GC pass and then check the delete flag
    rt_.GCNow();
    ASSERT_EQ(1, deleted);
}

TEST_F(GCTests, test6) {
    auto cx = rt_.NewContext();         
    
    JSAutoRequest ar(*cx);
    if (true)
    //if (deleted != 10)
    {
        //JSAutoRequest ar(*cx);
        
        JS::RootedObject obj(*cx, JS_NewObject(*cx, &klass, JS::NullPtr(), JS::NullPtr()));
        JS_NewObject(*cx, &klass, JS::NullPtr(), JS::NullPtr());

        // force a GC pass and then check the delete flag
        rt_.GCNow();
        ASSERT_EQ(1, deleted);
        
        //doIt(*cx);
    }
    
    // force a GC pass and then check the delete flag
    rt_.GCNow();
    rt_.GCNow();
    ASSERT_EQ(2, deleted);
}
#endif