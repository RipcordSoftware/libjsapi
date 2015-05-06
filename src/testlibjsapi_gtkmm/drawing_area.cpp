#include "drawing_area.h"
#include "image_surface.h"

#include <cstring>

DrawingArea::DrawingArea(rs::jsapi::Runtime& rt, Gtk::DrawingArea* area) : rt_(rt), obj_(rt), 
        area_(area), widget_(area, obj_) {
    auto functions = widget_.GetFunctions();
    
    functions.emplace_back("setSourceRgb", std::bind(&DrawingArea::SetSourceRgb, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("setLineWidth", std::bind(&DrawingArea::SetLineWidth, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("moveTo", std::bind(&DrawingArea::MoveTo, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("lineTo", std::bind(&DrawingArea::LineTo, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("onDraw", std::bind(&DrawingArea::OnDraw, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("stroke", std::bind(&DrawingArea::Stroke, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("createImageSurface", std::bind(&DrawingArea::CreateImageSurface, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("createImageSurfaceFromPng", std::bind(&DrawingArea::CreateImageSurfaceFromPng, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("createImageData", std::bind(&DrawingArea::CreateImageSurface, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("putImageData", std::bind(&DrawingArea::PutImageData, this, std::placeholders::_1, std::placeholders::_2));

    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, 
        functions, std::bind(&DrawingArea::Finalizer, this), obj_);
    
    area_->signal_draw().connect(sigc::mem_fun(*this, &DrawingArea::OnGtkDraw));
}

void DrawingArea::MoveTo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (onDrawContext_ && args.size() >= 2) {
        onDrawContext_->move_to(args[0].toNumber(), args[1].toNumber());
    }
    result = *this;
}

void DrawingArea::LineTo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (onDrawContext_ && args.size() >= 2) {
        onDrawContext_->line_to(args[0].toNumber(), args[1].toNumber());
    }
    result = *this;
}

void DrawingArea::SetSourceRgb(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (onDrawContext_ && args.size() >= 3) {
        onDrawContext_->set_source_rgb(args[0].toNumber(), args[1].toNumber(), args[2].toNumber());
    }
    result = *this;
}

void DrawingArea::SetLineWidth(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (onDrawContext_ && args.size() >= 1) {
        onDrawContext_->set_line_width(args[0].toNumber());
    }
    result = *this;  
}

void DrawingArea::Stroke(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (onDrawContext_) {
        onDrawContext_->stroke();
    }
    result = *this;
}

bool DrawingArea::OnGtkDraw(const Cairo::RefPtr<Cairo::Context>& cr) {
    if (onDraw_.isFunction()) {
        onDrawContext_ = cr;
               
        rs::jsapi::FunctionArguments args(rt_);
        args.Append(obj_);
        args.Append(area_->get_allocated_width());
        args.Append(area_->get_allocated_height());
        rs::jsapi::Value result(rt_);
        onDraw_.CallFunction(args, result);
        
        // TODO: if the function call throws then this will leak
        onDrawContext_.clear();
    } else if (image_) {
        cr->set_source(image_, 0, 0);
        cr->paint();
    }

    return true;
}

void DrawingArea::OnDraw(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    if (args.size() > 0) {
        if (args[0].isFunction()) {
            onDraw_ = args[0].toFunction();            
        } else {
            onDraw_.setUndefined();
        }
    }
    result = *this;
}

void DrawingArea::CreateImageSurface(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {    
    if (!image_) {
        auto width = area_->get_allocated_width();
        auto height = area_->get_allocated_height();
        
        if (args.size() >= 1 && args[0].isInt32()) {
            width = args[0].toInt32();
        }
        
        if (args.size() >= 2 && args[1].isInt32()) {
            height = args[1].toInt32();
        }
        
        image_ = Cairo::ImageSurface::create(Cairo::Format::FORMAT_ARGB32, area_->get_allocated_width(), area_->get_allocated_height());
        result = *(new ImageSurface(rt_, width, height, 4));
    }        
}

void DrawingArea::CreateImageSurfaceFromPng(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (!image_ && args.size() > 0 && args[0].isString()) {
        image_ = Cairo::ImageSurface::create_from_png(args[0].ToString());
        result = true;
    }        
}

void DrawingArea::PutImageData(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (args.size() >= 3) {    
        auto destData = image_->get_data();        
        const auto destWidth = image_->get_width();
        const auto destHeight = image_->get_height();
        const auto destStride = image_->get_stride();
        
        const unsigned char* sourceData = nullptr;
        unsigned sourceWidth = 0, sourceHeight = 0, sourceDepth = 0;
        ImageSurface::GetData(args[0], sourceData, sourceWidth, sourceHeight, sourceDepth);

        auto destX = args[1].toInt32();
        auto destY = args[2].toInt32();
        
        auto sourceX = 0;
        if (destX < 0) {
            sourceX = -destX;
            destX = 0;
            sourceWidth -= sourceX;
        }
        
        auto sourceY = 0;
        if (destY < 0) {
            sourceY = -destY;
            destY = 0;
            sourceHeight -= sourceY;
        }
        
        auto height = (destHeight - destY) < sourceHeight ? (destHeight - destY) : sourceHeight;
        
        if (destX == 0 && sourceWidth == destWidth) {                        
            auto destOffset = destData + (destY * destStride);
            auto sourceOffset = sourceData + (sourceY * sourceWidth * sourceDepth);
            std::memcpy(destOffset, sourceOffset, sourceWidth * height * sourceDepth);
            
            image_->mark_dirty(0, destY, sourceWidth, height);
            area_->queue_draw_area(0, destY, sourceWidth, height);
        } else {
            auto width = (destWidth - destX) < sourceWidth ? (destWidth - destX) : sourceWidth;
            for (unsigned i = 0; i < height; ++i) {
                auto destOffset = destData + ((destY + i) * destStride) + (destX * sourceDepth);
                auto sourceOffset = sourceData + (i * sourceWidth * sourceDepth);
                std::memcpy(destOffset, sourceOffset, width * sourceDepth);
            }
            
            image_->mark_dirty(destX, destY, width, height);
            area_->queue_draw_area(destX, destY, width, height);
        }
    }
}