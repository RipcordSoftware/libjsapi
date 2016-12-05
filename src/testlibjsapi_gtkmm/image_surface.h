/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Ripcord Software Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/

#ifndef IMAGE_SURFACE_H
#define	IMAGE_SURFACE_H

#include <vector>
#include <atomic>

#include "libjsapi.h"

#include "widget.h"

class ImageSurface {
public:
    ImageSurface(rs::jsapi::Context& cx, unsigned width, unsigned height, unsigned depth = 4);        
    
    operator rs::jsapi::Value&() { return obj_; }
    
    static bool GetData(const rs::jsapi::Value& obj, const unsigned char*& data, unsigned& width, unsigned& height, unsigned& depth);
    
private:
    void Finalizer() {
        if (--refCount_ == 0) {
            delete this;
        }
    }
    
    void GetCallback(const char* name, rs::jsapi::Value& value);
    
    rs::jsapi::Context& cx_;
    rs::jsapi::Value obj_;
    unsigned width_;
    unsigned height_;
    unsigned depth_;
    
     rs::jsapi::Value array_;
     std::vector<unsigned char> data_;
     
     std::atomic<int> refCount_;
};

#endif	/* IMAGE_SURFACE_H */

