#ifndef IMAGE_SURFACE_H
#define	IMAGE_SURFACE_H

#include <vector>
#include <atomic>

#include "libjsapi.h"

#include "widget.h"

class ImageSurface {
public:
    ImageSurface(rs::jsapi::Runtime& rt, unsigned width, unsigned height, unsigned depth = 4);
    
    bool GetCallback(const char* name, rs::jsapi::Value& value);
    
    operator rs::jsapi::Value&() { return obj_; }
    
    static bool GetData(const rs::jsapi::Value& obj, const unsigned char*& data, unsigned& width, unsigned& height, unsigned& depth);
    
private:    
    void Finalizer() {
        if (--refCount_ == 0) {
            delete this;
        }
    }
    
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    unsigned width_;
    unsigned height_;
    unsigned depth_;
    
     rs::jsapi::Value array_;
     std::vector<unsigned char> data_;
     
     std::atomic<int> refCount_;
};

#endif	/* IMAGE_SURFACE_H */

