#include "button.h"

Button::Button(rs::jsapi::Runtime& rt, Gtk::Button* button) : rt_(rt), button_(button), obj_(rt), widget_(button, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("setText", std::bind(&Button::SetLabel, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("setLabel", std::bind(&Button::SetLabel, this, std::placeholders::_1, std::placeholders::_2));        

    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, 
        functions, std::bind(&Button::Finalizer, this), obj_);
}

void Button::SetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    button_->set_label(args[0].ToString());
    result = *this;
}