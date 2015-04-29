#include "widget.h"

Widget::Widget(Gtk::Widget* widget, rs::jsapi::Value& parent) : widget_(widget), parent_(parent) {}
    
void Widget::Show(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    widget_->show();
    result = parent_;
}

void Widget::Hide(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    widget_->hide();
    result = parent_;
}

void Widget::GetWidth(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = widget_->get_width();
    result = parent_;
}

void Widget::GetHeight(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = widget_->get_height();
    result = parent_;
}

void Widget::GetOpacity(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = widget_->get_opacity();
    result = parent_;
}

void Widget::SetOpacity(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    widget_->set_opacity(args[0].toNumber());
    result = parent_;
}

void Widget::GetName(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = widget_->get_name();
    result = parent_;
}

void Widget::SetName(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    widget_->set_name(args[0].ToString());
    result = parent_;
}