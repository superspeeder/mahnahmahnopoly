#pragma once

#include "kat/common.hpp"

namespace kat {

    class SpriteBatch {
    public:

        static constexpr size_t kMaxBufferSize = 128000000;
        static constexpr size_t kMaxFloatsPerBuffer = kMaxBufferSize / sizeof(float);
        static constexpr size_t kVertexSize = 2 + 2 + 4;
        static constexpr size_t kMaxVerticesPerBuffer = kMaxFloatsPerBuffer / kVertexSize;
        static constexpr size_t kMaxQuadsPerBuffer = kMaxVerticesPerBuffer / 4;

        SpriteBatch();
        ~SpriteBatch();

        void begin();
        void end();


    private:

        size_t m_QuadsBatched;
        float m_BufferData[kMaxFloatsPerBuffer];

        uint32_t m_VertexBuffer;

        inline static bool s_IndexBufferInitialized = false;
        static uint32_t m_IndexBuffer;

    };

} // kat
