#ifndef CHECK_BUTTON_H
#define	CHECK_BUTTON_H

#include "libjsapi.h"

#include "widget.h"

class CheckButton {
public:
    CheckButton(rs::jsapi::Runtime& rt, Gtk::CheckButton* button);
    
    void GetActive(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void SetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    void OnClick(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    operator rs::jsapi::Value&() { return obj_; }

private:
    void Finalizer() {
        delete this;
    }
    
    void OnButtonClicked();
    
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Gtk::CheckButton* button_;    
    Widget widget_; 
    
    rs::jsapi::Value onClick_ { rt_ };

};

#endif	/* CHECK_BUTTON_H */

