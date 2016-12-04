#include "button.h"

Button::Button(rs::jsapi::Context& cx, Gtk::Button* button) : cx_(cx), button_(button), obj_(cx), widget_(button, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("getLabel", std::bind(&Button::SetLabel, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("setLabel", std::bind(&Button::SetLabel, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("onClick", std::bind(&Button::OnClick, this, std::placeholders::_1, std::placeholders::_2)); 
    functions.emplace_back("setFocus", std::bind(&Button::SetFocus, this, std::placeholders::_1, std::placeholders::_2)); 
    functions.emplace_back("focus", std::bind(&Button::SetFocus, this, std::placeholders::_1, std::placeholders::_2)); 

    rs::jsapi::Object::Create(cx, {}, nullptr, nullptr, 
        functions, std::bind(&Button::Finalizer, this), obj_);
}

void Button::GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = button_->get_label();
}

void Button::SetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    button_->set_label(args[0].ToString());
    result = *this;
}

void Button::SetFocus(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    button_->get_toplevel()->set_focus_child(*button_);
    result = *this;
}

void Button::OnClick(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    if (args.size() > 0) {
        if (args[0].isFunction()) {
            onClick_ = args[0].toFunction();
            button_->signal_clicked().connect(sigc::mem_fun(*this, &Button::OnButtonClicked));
        } else {
            onClick_.setUndefined();
        }
    }
    result = *this;
}

void Button::OnButtonClicked() {
    if (onClick_.isFunction()) {   
        rs::jsapi::FunctionArguments args(cx_);
        rs::jsapi::Value result(cx_);
        onClick_.CallFunction(args, result);
    }
}