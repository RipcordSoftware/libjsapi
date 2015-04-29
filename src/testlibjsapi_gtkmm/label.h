#ifndef LABEL_H
#define	LABEL_H

#include "libjsapi.h"

#include "widget.h"

class Label {
public:

    Label(rs::jsapi::Runtime& rt, Gtk::Label* label);
    
    void SetText(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    void Finalizer() {
        delete this;
    }
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Gtk::Label* label_;    
    Widget widget_;    
};

#endif	/* LABEL_H */

