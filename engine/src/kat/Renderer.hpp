#pragma once

namespace kat {

    class Renderer;


    class Renderable {
    public:

        virtual void render(Renderer* renderer) = 0;

    private:
    };

    class Renderer {
    public:

        Renderer();
        ~Renderer();


    private:


    };

} // kat
