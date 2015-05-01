#include "button.h"

Button::Button(rs::jsapi::Runtime& rt, Gtk::Button* button) : rt_(rt), button_(button), obj_(rt), widget_(button, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("setText", std::bind(&Button::SetLabel, this, std::placeholders::_1, std::placeholders::_2));
    functions.emplace_back("setLabel", std::bind(&Button::SetLabel, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("onClick", std::bind(&Button::OnClick, this, std::placeholders::_1, std::placeholders::_2)); 

    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, 
        functions, std::bind(&Button::Finalizer, this), obj_);
}

void Button::SetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    button_->set_label(args[0].ToString());
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
        rs::jsapi::FunctionArguments args(rt_);
        rs::jsapi::Value result(rt_);
        onClick_.CallFunction(args, result);
    }
}