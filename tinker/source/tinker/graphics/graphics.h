#pragma once

#include <stddef.h>



namespace ti::graphics
{
    void Initalize();
    void Terminate();

    class BufferView;

    struct BufferDesc
    {
        size_t size;
    };

    class Buffer
    {
    public:
        Buffer(BufferDesc desc);
        ~Buffer();

        const BufferDesc& GetDesc() const;
    
    private:
        BufferDesc m_Desc;
        void* m_Internal;
    };

    void DrawImage(...);

    struct TextureDesc
    {

    };

    using BufferHandle = int;
    using TextureHandle = int;

    BufferHandle CreateBuffer(size_t size);
    void resizeBuffer(size_t newSize, BufferHandle handle);
    void destroyBuffer(BufferHandle handle);

    TextureHandle createTexture();

    static constexpr int MAX_VERTEX_ATTRIBUTE_COUNT = 16;
}