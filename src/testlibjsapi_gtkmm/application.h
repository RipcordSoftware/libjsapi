#ifndef APPLICATION_H
#define	APPLICATION_H

#include <gtkmm.h>

#include "libjsapi.h"

class Application {
public:
    Application(rs::jsapi::Runtime& rt, const char* appName, int argc = 0, char** argv = nullptr);
    
    void Run(const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result);
    
    operator rs::jsapi::Value&() { return obj_; }
    
private:
    void Finalizer() {
        delete this;
    }
    
    rs::jsapi::Runtime& rt_;
    rs::jsapi::Value obj_;
    Glib::RefPtr<Gtk::Application> app_;
};

#endif	/* APPLICATION_H */

