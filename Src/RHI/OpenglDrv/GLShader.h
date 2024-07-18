#pragma once 

#include <string>
#include <vector>
#include <memory>

//#include "RHI/Shader.h"

namespace PPRHI {
    class GLShader { //: public Shader{
    public:
        class Source {
        public:
            ~Source();

        protected:
            explicit Source(const char* fileName);
            Source(unsigned int type, const std::string& text);

            unsigned int type_{ 0 };
            unsigned int shaderHandle_{ 0 };
            friend class GLShader;
        };
    public:
        GLShader(const std::shared_ptr<Source>& a);
        GLShader(const std::shared_ptr<Source>& a, const std::shared_ptr<Source>& b);
        GLShader(const std::shared_ptr<Source>& a, const std::shared_ptr<Source>& b, const std::shared_ptr<Source>& c);
        GLShader(const std::shared_ptr<Source>& a, const std::shared_ptr<Source>& b, const std::shared_ptr<Source>& c,
            const std::shared_ptr<Source>& d);
        GLShader(const std::shared_ptr<Source>& a, const std::shared_ptr<Source>& b, const std::shared_ptr<Source>& c,
            const std::shared_ptr<Source>& d, const std::shared_ptr<Source>& e);
        ~GLShader();

        static std::shared_ptr<Source> CreateSource(const char* fileName);
        static std::shared_ptr<Source> CreateSource(unsigned int type, const std::string& text);

        void Use();

    private:
        void Compile();

    private:
        std::vector<std::shared_ptr<Source>> sources_;
        unsigned int programHandle_{ 0 };
    };
}
