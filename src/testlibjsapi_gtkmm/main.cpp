#include <gtkmm.h>

#include <string>
#include <fstream>
#include <iostream>

#include "libjsapi.h"

#include "application.h"
#include "window.h"
#include "builder.h"

std::string LoadScript(int argc, char** argv) {
    std::string script;
    
    for (int i = 1; i < argc; i++) {
        std::fstream file;
        file.open(argv[i], std::ios::in);
        while (!!file) {
            char buffer[4096];
            file.read(buffer, sizeof(buffer));
            script.append(buffer, file.gcount());
            script += ";\n";
        }
    }
    
    return std::move(script);
}

int main(int argc, char** argv) {  
 
    try {
        std::string script = LoadScript(argc, argv);

        rs::jsapi::Runtime rt(512 * 1024 * 1024);

        auto app = new Application(rt, "com.ripcordsoftware.examples.gtk", 1, argv);
        rs::jsapi::Global::DefineProperty(rt, "app", *app);

        auto builder = new Builder(rt);
        rs::jsapi::Global::DefineProperty(rt, "builder", *builder);
        
        rs::jsapi::Global::DefineFunction(rt, "trace", 
            [](JSContext *cx, unsigned argc, JS::Value *vp){
                auto args = JS::CallArgsFromVp(argc, vp);
                
                for (int i = 0; i < args.length(); i++) {
                    rs::jsapi::Value value(cx, args[i]);
                    std::cout << value.ToString();
                }
                
                if (args.length() > 0) {
                    std::cout << std::endl;
                }
                
                return true;
            });

        rt.Evaluate(script.c_str());
    } catch (const rs::jsapi::ScriptException& ex) {
        std::cerr << 
            "ERROR: line " << ex.lineno << std::endl <<
            ex.what() << std::endl <<
            ex.linebuf << std::endl;
        
    }
    
    return 0;
}