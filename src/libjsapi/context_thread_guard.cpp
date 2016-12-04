#include "context_thread_guard.h"

#include <algorithm>

#include "exceptions.h"

std::vector<std::thread::id> rs::jsapi::ContextThreadGuard::activeThreads_;
std::mutex rs::jsapi::ContextThreadGuard::activeThreadsLock_;

rs::jsapi::ContextThreadGuard::ContextThreadGuard(std::thread::id id) {
    if (!AddThread(id)) {
        throw RuntimeThreadInstanceException();
    }
}

bool rs::jsapi::ContextThreadGuard::AddThread(std::thread::id id) {
    std::lock_guard<std::mutex> lock(activeThreadsLock_);
    
    auto iter = std::find(activeThreads_.cbegin(), activeThreads_.cend(), id);
    bool add = iter == activeThreads_.cend();
    if (add) {
        activeThreads_.push_back(id);
    }
    
    return add;
}

bool rs::jsapi::ContextThreadGuard::RemoveThread(std::thread::id id) {
    std::lock_guard<std::mutex> lock(activeThreadsLock_);   
    
    auto iter = std::find(activeThreads_.begin(), activeThreads_.end(), id);
    bool remove = iter != activeThreads_.end();
    if (remove) {
        activeThreads_.erase(iter);
    }
    
    return remove;
}

bool rs::jsapi::ContextThreadGuard::RemoveThread() {
    return RemoveThread(std::this_thread::get_id());
}