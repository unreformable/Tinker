#pragma once

#include "glad/glad.h"



namespace ti::graphics::ogl
{
    /**
     * @param usage Refer to: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
     * @return The handle of created buffer.
    */
    GLuint CreateBuffer(GLsizeiptr size, const void* data, GLenum usage);

    /**
     * @param type Refer to: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
     * @return The handle of created shader.
     * @retval 0 if could not open file.
     * @retval 0 if could not compile shader.
    */
    GLuint CreateShader(const char* filePath, GLenum type);

    /**
     * @return The handle of created program.
     * @retval 0 if could not open file.
     * @retval 0 if could not compile program.
    */
    GLuint CreateProgram(const char* vsFilePath, const char* fsFilePath);
    //glProgramUniformXXX();

    /**
     * @param mipmaps Total count of mipmaps for texture including main level.
     * @param internalFormat Refer to: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
     * @param format, pixelDataType Refer to: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml
     * @param minFilter One of these: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR.
     * @param magFilter One of these: GL_NEAREST, GL_LINEAR.
     * @param wrapModeX, wrapModeY One of these: GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE.
     * @param borderColor The rgba color (0-1 range) of border when using GL_CLAMP_TO_BORDER wrap mode.
    */
    GLuint CreateTexture2D(const void* pixels, GLsizei width, GLsizei height, GLsizei mipmaps, GLenum internalFormat, GLenum format, GLenum pixelDataType, GLenum minFilter, GLenum magFilter, GLenum wrapModeX, GLenum wrapModeY);
    //glBindTextureUnit(unit, texture);
}