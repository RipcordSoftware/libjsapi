#ifndef DRAWING_AREA_H
#define	DRAWING_AREA_H

#include "libjsapi.h"

#include "widget.h"

class DrawingArea {
public:
    DrawingArea(rs::jsapi::Runtime& rt, Gtk::DrawingArea* entry);    
    
    void SetSourceRgb(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetLineWidth(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void MoveTo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void LineTo(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void Stroke(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void OnDraw(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);        
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    void Finalizer() {
        delete this;
    }
    
    bool OnGtkDraw(const Cairo::RefPtr<Cairo::Context>& cr);
    
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Gtk::DrawingArea* area_;    
    Widget widget_; 
    
    Cairo::RefPtr<Cairo::Context> onDrawContext_;
    rs::jsapi::Value onDraw_ { rt_ };
};

#endif	/* DRAWING_AREA_H */

