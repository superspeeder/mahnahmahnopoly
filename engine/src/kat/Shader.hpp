#pragma once

#include "kat/common.hpp"

namespace kat {

    class Shader {
    public:
        Shader();
        ~Shader();

        void use();

        void uniform1f(const std::string& name, float x);
        void uniform2f(const std::string& name, float x, float y);
        void uniform3f(const std::string& name, float x, float y, float z);
        void uniform4f(const std::string& name, float x, float y, float z, float w);

    private:

    };

} // kat
