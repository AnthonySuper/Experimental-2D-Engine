#ifndef _SCRIPT_ENGINE_HPP
#define _SCRIPT_ENGINE_HPP
#include <mruby.h>
#include <string>
#include <mruby/compile.h>
#include "errors.hpp"
#include <stdio.h>


namespace NM {
    class ScriptEngine {
        
    public:
        ScriptEngine();
         mrb_state* getMRB();
        
        /**
         Run a line of ruby code and return the result.
         */
        mrb_value runString(std::string line);
        
        /**
         Load a file into the scripting engine.
         Returns the result of the file load.
         */
        mrb_value loadPath(std::string path);
        
    private:
        mrb_state *mrb;
    };
}
#endif