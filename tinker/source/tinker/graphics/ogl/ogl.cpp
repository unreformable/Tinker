#include "tinker/graphics/ogl/ogl.h"

#include "tinker/core/debug.h"
#include "tinker/core/file.h"



namespace ti::graphics::ogl
{
    GLuint CreateBuffer(GLsizeiptr size, const void* data, GLenum usage)
    {
        GLuint buffer;
        glCreateBuffers(1, &buffer);
        glNamedBufferData(buffer, size, data, usage);

        return buffer;
    }

    GLuint CreateShader(const char* filePath, GLenum type)
    {
        std::vector<u8> source = core::readFile(filePath);
        if(source.empty())
        {
            TI_ERROR("Could not open file %s", filePath);
            return 0;
        }

        GLuint shader = glCreateShader(type);
        const GLchar* pSource = (GLchar*)source.data();
        const GLint sourceLength = source.size();
        glShaderSource(shader, 1, &pSource, &sourceLength);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
#ifndef NDEBUG
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            TI_ERROR("Could not compile shader. Info: %s", infoLog);
#endif
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLuint CreateProgram(const char* vsFilePath, const char* fsFilePath)
    {
        GLuint vs = CreateShader(vsFilePath, GL_VERTEX_SHADER);
        if(vs == 0)
        {
            TI_ERROR("Could not create vertex shader");
            return 0;
        }
        GLuint fs = CreateShader(fsFilePath, GL_FRAGMENT_SHADER);
        if(fs == 0)
        {
            TI_ERROR("Could not create fragment shader");
            return 0;
        }

        GLuint program;
        program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);

        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success)
        {
#ifndef NDEBUG
            GLchar infoLog[512];
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            TI_ERROR("Could not link program. Info: %s", infoLog);
#endif
            glDeleteProgram(program);
            return 0;
        }

        return program;
    }

    GLuint CreateTexture2D(const void* pixels, GLsizei width, GLsizei height, GLsizei mipmaps, GLenum internalFormat, GLenum format, GLenum pixelDataType, GLenum minFilter, GLenum magFilter, GLenum wrapModeX, GLenum wrapModeY)
    {
        TI_REQUIRE(mipmaps >= 1, "At least 1 mipmap");
        TI_REQUIRE(width > 0, "Width greater than 0");
        TI_REQUIRE(height > 0, "Height greater than 0");
        
        GLuint texture;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);

        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrapModeX);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrapModeY);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, minFilter);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, magFilter);

        glTextureStorage2D(texture, mipmaps, internalFormat, width, height); // GL_RGBA8
        glTextureSubImage2D(texture, 0, 0, 0, width, height, format, pixelDataType, pixels); // GL_RGBA, GL_UNSIGNED_BYTE

        if(mipmaps > 1)
            glGenerateTextureMipmap(texture);

        return texture;
    }
}