#include "image_surface.h"

#include <cstring>
#include <algorithm>

ImageSurface::ImageSurface(rs::jsapi::Context& cx, unsigned width, unsigned height, unsigned depth) :
        cx_(cx), obj_(cx), width_(width), height_(height), depth_(depth), array_(cx), refCount_(2),
        data_(width_ * height_ * depth_, 0) {

    rs::jsapi::Object::Create(cx, { "data", "width", "height", "depth" }, 
        std::bind(&ImageSurface::GetCallback, this, std::placeholders::_1, std::placeholders::_2),
        nullptr,
        {}, 
        std::bind(&ImageSurface::Finalizer, this), obj_);        
    
    rs::jsapi::Object::SetPrivate(obj_, typeid(ImageSurface).hash_code(), this);
    
    rs::jsapi::DynamicArray::Create(cx,
        [&](int index, rs::jsapi::Value& value) { value.set(data_[index]); },
        [&](int index, const rs::jsapi::Value& value) { 
            if (value.isInt32()) {
                data_[index] = value.toInt32();
            } else if (value.isNumber()) {
                data_[index] = value.toNumber();
            }
        },
        [&]() { return data_.size(); },
        std::bind(&ImageSurface::Finalizer, this),
        array_);
}

void ImageSurface::GetCallback(const char* name, rs::jsapi::Value& value) {
    if (std::strcmp(name, "data") == 0) {
        value = array_;
    } else if (std::strcmp(name, "width") == 0) {
        value = (int)width_;
    } else if (std::strcmp(name, "height") == 0) {
        value = (int)height_;
    } else if (std::strcmp(name, "depth") == 0) {
        value = (int)depth_;
    }    
}

 bool ImageSurface::GetData(const rs::jsapi::Value& obj, const unsigned char*& data, unsigned& width, unsigned& height, unsigned& depth) {
    uint64_t dataType = 0;
    void* ptr = nullptr;
    if (rs::jsapi::Object::GetPrivate(obj, dataType, ptr) && dataType == typeid(ImageSurface).hash_code()) {
        auto that = reinterpret_cast<ImageSurface*>(ptr);
        data = that->data_.data();
        width = that->width_;
        height = that->height_;
        depth = that->depth_;
        return true;
    } else {
        return false;
    }
}