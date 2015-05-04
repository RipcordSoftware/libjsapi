#include "check_button.h"

CheckButton::CheckButton(rs::jsapi::Runtime& rt, Gtk::CheckButton* button) : rt_(rt), button_(button), obj_(rt), widget_(button, obj_) {
    auto functions = widget_.GetFunctions();

    functions.emplace_back("getActive", std::bind(&CheckButton::GetActive, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("getLabel", std::bind(&CheckButton::SetLabel, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("setLabel", std::bind(&CheckButton::SetLabel, this, std::placeholders::_1, std::placeholders::_2));        
    functions.emplace_back("onClick", std::bind(&CheckButton::OnClick, this, std::placeholders::_1, std::placeholders::_2)); 

    rs::jsapi::Object::Create(rt, {}, nullptr, nullptr, 
        functions, std::bind(&CheckButton::Finalizer, this), obj_);
}

void CheckButton::GetActive(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = button_->get_active();
}

void CheckButton::GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    result = button_->get_label();
}

void CheckButton::SetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    button_->set_label(args[0].ToString());
    result = *this;
}

void CheckButton::OnClick(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) { 
    if (args.size() > 0) {
        if (args[0].isFunction()) {
            onClick_ = args[0].toFunction();
            button_->signal_clicked().connect(sigc::mem_fun(*this, &CheckButton::OnButtonClicked));
        } else {
            onClick_.setUndefined();
        }
    }
    result = *this;
}

void CheckButton::OnButtonClicked() {
    if (onClick_.isFunction()) {   
        rs::jsapi::FunctionArguments args(rt_);
        rs::jsapi::Value result(rt_);
        onClick_.CallFunction(args, result);
    }
}