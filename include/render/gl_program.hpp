#ifndef _SHADER_HPP
#define _SHADER_HPP

#include "render/gl_include.hpp"
#include <string>
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "util.hpp"

namespace NM {
    class GLProgram {
    public:
        using SourceType = const std::string;
        GLProgram(SourceType vertex, SourceType fragment);
        GLProgram(const GLProgram&);
        
        static GLProgram fromFiles(std::string vertPath, std::string fragPath);
        
        class CompileError : public std::runtime_error {
        public:
            CompileError(std::string& what) : std::runtime_error(what){};
        };
        
        void set(std::string name, GLfloat val);
        void set(std::string name, GLint value);
        void set(std::string name, glm::mat4 m);
        void set(std::string name, glm::vec4 v);
        
        void use();
        
        inline GLuint getId() { return id; }
        
    private:
        GLint getLoc(std::string name);
        GLuint id;
    };
}
#endif