#include "builder.h"

#include <exception>

#include "window.h"
#include "button.h"
#include "check_button.h"
#include "label.h"
#include "entry.h"
#include "drawing_area.h"

class BuilderException : public std::exception {
public:
    BuilderException(const char* msg) : msg_(msg) {}
    
    virtual const char* what() const noexcept override { return msg_.c_str(); } 
    
private:
    std::string msg_;
};

Builder::Builder(rs::jsapi::Context& cx) : cx_(cx), obj_(cx), null_(cx) {
    rs::jsapi::Object::Create(cx, {}, nullptr, nullptr, {
        { "addFromFile", std::bind(&Builder::AddFromFile, this, std::placeholders::_1, std::placeholders::_2) },
        { "getWidget", std::bind(&Builder::GetWidget, this, std::placeholders::_1, std::placeholders::_2) },
        { "getWindow", std::bind(&Builder::GetWindow, this, std::placeholders::_1, std::placeholders::_2) },
        { "getButton", std::bind(&Builder::GetButton, this, std::placeholders::_1, std::placeholders::_2) },
        { "getCheckButton", std::bind(&Builder::GetCheckButton, this, std::placeholders::_1, std::placeholders::_2) },
        { "getLabel", std::bind(&Builder::GetLabel, this, std::placeholders::_1, std::placeholders::_2) },
        { "getEntry", std::bind(&Builder::GetEntry, this, std::placeholders::_1, std::placeholders::_2) },
        { "getDrawingArea", std::bind(&Builder::GetDrawingArea, this, std::placeholders::_1, std::placeholders::_2) }
    }, std::bind(&Builder::Finalizer, this), obj_);
}

void Builder::AddFromFile(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    if (args.size() > 0 && args[0].isString()) {
        builder_ = Gtk::Builder::create();
        
        try {
            builder_->add_from_file(args[0].ToString());             
            result = *this;
        } catch (const Glib::Exception& ex) {
            throw BuilderException(ex.what().c_str());
        }
    }    
}

void Builder::GetWindow(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = FindWidget(args);
    if (widget && GTK_IS_WINDOW(widget->gobj())) {
        auto window = reinterpret_cast<Gtk::Window*>(widget);
        result = *(new Window(cx_, window));
    }
}

void Builder::GetButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = FindWidget(args);
    if (widget && GTK_IS_BUTTON(widget->gobj())) {
        auto button = reinterpret_cast<Gtk::Button*>(widget);
        result = *(new Button(cx_, button));
    }
}

void Builder::GetCheckButton(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = FindWidget(args);
    if (widget && GTK_IS_CHECK_BUTTON(widget->gobj())) {
        auto button = reinterpret_cast<Gtk::CheckButton*>(widget);
        result = *(new CheckButton(cx_, button));
    }
}

void Builder::GetLabel(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = FindWidget(args);
    if (widget && GTK_IS_LABEL(widget->gobj())) {
        auto label = reinterpret_cast<Gtk::Label*>(widget);
        result = *(new Label(cx_, label));
    }
}

void Builder::GetEntry(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = FindWidget(args);
    if (widget && GTK_IS_ENTRY(widget->gobj())) {
        auto label = reinterpret_cast<Gtk::Entry*>(widget);
        result = *(new Entry(cx_, label));
    }
}

void Builder::GetDrawingArea(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = FindWidget(args);
    if (widget && GTK_IS_DRAWING_AREA(widget->gobj())) {
        auto area = reinterpret_cast<Gtk::DrawingArea*>(widget);
        result = *(new DrawingArea(cx_, area));
    }
}

Gtk::Widget* Builder::FindWidget(const std::vector<rs::jsapi::Value>& args) {
    Gtk::Widget* widget = nullptr;
    if (args.size() > 0 && args[0].isString()) {        
        builder_->get_widget(args[0].ToString(), widget);
    }
    return widget;
}

void Builder::GetWidget(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result) {
    auto widget = FindWidget(args);
    if (widget && GTK_IS_CHECK_BUTTON(widget->gobj())) {
        GetCheckButton(args, result);
    } else if (widget && GTK_IS_BUTTON(widget->gobj())) {
        GetButton(args, result);
    } else if (widget && GTK_IS_ENTRY(widget->gobj())) {
        GetEntry(args, result);
    } else if (widget && GTK_IS_LABEL(widget->gobj())) {
        GetLabel(args, result);
    } else if (widget && GTK_IS_WINDOW(widget->gobj())) {
        GetWindow(args, result);
    } else if (widget && GTK_IS_DRAWING_AREA(widget->gobj())) {
        GetDrawingArea(args, result);
    }
}