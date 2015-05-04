#include <cairomm-1.0/cairomm/context.h>

#include "drawing_area.h"

DrawingArea::DrawingArea(rs::jsapi::Runtime& rt, Gtk::DrawingArea* area) : rt_(rt), obj_(rt), 
        area_(area), widget_(area, obj_) {
    auto functions = widget_.GetFunctions();
    
    functions.emplace_back("setSourceRgb", std::bind(&DrawingArea::SetSourceRgb, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("setLineWidth", std::bind(&DrawingArea::SetLineWidth, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("moveTo", std::bind(&DrawingArea::MoveTo, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("lineTo", std::bind(&DrawingArea::LineTo, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("onDraw", std::bind(&DrawingArea::OnDraw, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("stroke", std::bind(&DrawingArea::Stroke, this, std::placeholders::_1, std::placeholders::_2));

    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, 
        functions, std::bind(&DrawingArea::Finalizer, this), obj_);         
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
    }

    return true;
}

void DrawingArea::OnDraw(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    if (args.size() > 0) {
        if (args[0].isFunction()) {
            onDraw_ = args[0].toFunction();
            area_->signal_draw().connect(sigc::mem_fun(*this, &DrawingArea::OnGtkDraw));
        } else {
            onDraw_.setUndefined();
        }
    }
    result = *this;
}