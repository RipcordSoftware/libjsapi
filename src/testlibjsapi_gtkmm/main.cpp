#include <gtkmm.h>

#include <memory>
#include <vector>

#include "libjsapi.h"

#include "window.h"

int main(int argc, char** argv) {

    auto app = Gtk::Application::create(argc, argv, "com.ripcordsoftware.examples.gtk");            
    
    rs::jsapi::Runtime rt;
    Window mainWindow(rt);
      
    rs::jsapi::Global::DefineProperty(rt, "window", mainWindow);
    rt.Evaluate(
        "window.setDefaultSize(10, 10); "
        "window.setTitle('This is a TEST!'); "
        "window.setBorderWidth(100); "
        //"var label = window.addLabel('myLabel'); "
        //"label && label.setText('hello').setOpacity(0.6).show();"
        "var button = window.addButton('myButton');"
        "button && button.setText('Press me!').show();"
    );

    return app->run(mainWindow);    
}