#ifndef BUILDER_H
#define	BUILDER_H

#include <gtkmm.h>

#include "libjsapi.h"

class Builder {
public:
    Builder(rs::jsapi::Runtime& rt);
    
    void AddFromFile(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetWidget(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetWindow(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetCheckButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetEntry(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    void GetDrawingArea(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);    
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    Gtk::Widget* FindWidget(const std::vector<rs::jsapi::Value>& args);
    
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Glib::RefPtr<Gtk::Builder> builder_;
    
    rs::jsapi::Value null_;
};

#endif	/* BUILDER_H */

