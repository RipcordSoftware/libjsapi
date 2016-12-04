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
        }
        
        script += ";\n";
    }
    
    return script;
}

int main(int argc, char** argv) {  
 
    try {
        std::string script = LoadScript(argc, argv);

        rs::jsapi::Context cx(1024 * 1024 * 1024, true, true);

        auto app = new Application(cx, "com.ripcordsoftware.examples.gtk", 1, argv);
        rs::jsapi::Global::DefineProperty(cx, "app", *app);

        auto builder = new Builder(cx);
        rs::jsapi::Global::DefineProperty(cx, "builder", *builder);
        
        rs::jsapi::Global::DefineFunction(cx, "trace", 
            [](const std::vector<rs::jsapi::Value>& args, rs::jsapi::Value& result){
                for (auto arg : args) {
                    std::cout << arg.ToString();
                }
                
                if (args.size() > 0) {
                    std::cout << std::endl;
                }
                
                return true;
            });

        cx.Evaluate(script.c_str());
    } catch (const rs::jsapi::ScriptException& ex) {
        std::cerr <<
            "ERROR: line " << ex.lineno << std::endl <<
            ex.what() << std::endl;
    }
    
    return 0;
}