#ifndef BUTTON_H
#define	BUTTON_H

#include "libjsapi.h"

#include "widget.h"

class Button {
public:

    Button(rs::jsapi::Runtime& rt, Gtk::Button* button);
    
    void SetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    void Finalizer() {
        delete this;
    }
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Gtk::Button* button_;    
    Widget widget_;    
};

#endif	/* BUTTON_H */

