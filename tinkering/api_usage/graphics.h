#pragma once

#include <stddef.h>



namespace graphics
{
    struct BufferDesc
    {
        size_t size;
        // binding
        // attributes
    };

    // 
    class VertexBuffer
    {
    public:
        VertexBuffer(Device* device, const BufferDesc& desc);
        ~VertexBuffer();
    };

    struct PipelineDesc
    {
        // shader stages
    };

    class Pipeline
    {
    public:
        Pipeline(const PipelineDesc& desc);
        ~Pipeline();
    };

    struct TextureDesc
    {

    };

    class Texture
    {
    public:
        Texture(const TextureDesc& desc);
        ~Texture();
    }

    class Device
    {
    public:
        VertexBuffer CreateBuffer(const BufferDesc& desc);
    };
}