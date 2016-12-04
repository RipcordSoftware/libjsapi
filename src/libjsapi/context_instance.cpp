#include "context_instance.h"

#include <memory>
#include <cstdlib>

#include <js/Initialization.h>

#ifdef __GNUC__
#define INIT_PRIORITY(N) __attribute__ ((init_priority (N + 101)))
#else
#error "Unable to set static initiaiization order on this platform. If you wish to fix this for your compiler please PR the change."
#endif

std::atomic<bool> rs::jsapi::ContextInstance::initCalled_ INIT_PRIORITY(0);
std::atomic<int> rs::jsapi::ContextInstance::count_ INIT_PRIORITY(0);
std::mutex rs::jsapi::ContextInstance::m_ INIT_PRIORITY(0);

rs::jsapi::ContextInstance::ContextInstance() {
    if (count_ == 0 && !initCalled_) {
        std::lock_guard<std::mutex> lk(m_);
        if (count_ == 0 && !initCalled_) {
            JS_Init();           
            
            // shutdown JS at process end
            std::atexit([] { JS_ShutDown(); });

            // keep track of the number of instances
            ++count_;

            // don't allow init to be called more than once
            initCalled_= true;
        } else {
            ++count_;
        }
    } else {
        ++count_;
    }   
}

rs::jsapi::ContextInstance::~ContextInstance() {    
    --count_;
}