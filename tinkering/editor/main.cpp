#include "tinker/core/debug.h"
#include "tinker/graphics/ogl/ogl.h"
#include "tinker/math/vector2.h"
#include "tinker/math/vector3.h"
#include "tinker/math/matrix4.h"
#include "tinker/math/trigonometry.h"
#include "glm/gtc/matrix_transform.hpp"

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <vector>



static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "Error: %s\n" << description << '\n';
}

static void openglErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const sourceStr = [source]()
    {
		switch (source)
		{
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
        TI_ASSERT(false, "");
        return "";
	}();

	auto const typeStr = [type]()
    {
		switch (type)
		{
            case GL_DEBUG_TYPE_ERROR: return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "MARKER";
            case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
        TI_ASSERT(false, "");
        return "";
	}();

	auto const severityStr = [severity]()
    {
		switch (severity)
        {
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW: return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
        TI_ASSERT(false, "");
        return "";
	}();
	std::cerr << sourceStr << ", " << typeStr << ", " << severityStr << ", " << id << ": " << message << '\n';
}

class Layer
{
public:
    virtual ~Layer() = default;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
};

class Application
{
public:
    Application()
    {
        glfwSetErrorCallback(glfwErrorCallback);

        if(!glfwInit())
        {
            assert(false);
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_Window = glfwCreateWindow(1280, 720, "Voxel", NULL, NULL);
        if(!m_Window)
        {
            glfwTerminate();
            assert(false);
        }
        glfwMakeContextCurrent(m_Window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(1);

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openglErrorCallback, nullptr);
    }

    ~Application()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Run()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        float dt = 0.0f;
        double last = glfwGetTime();
        while(!glfwWindowShouldClose(m_Window))
        {
            glfwPollEvents();
            
            for(Layer* layer : m_Layers)
            {
                layer->Update(dt);
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            for(Layer* layer : m_Layers)
            {
                layer->Render();
            }

            glfwSwapBuffers(m_Window);

            const double now = glfwGetTime();
            dt = (float)(now - last);
            last = now;
        }
    }

    void PushLayer(Layer* layer)
    {
        m_Layers.push_back(layer);
    }

private:
    GLFWwindow* m_Window;
    std::vector<Layer*> m_Layers;
};

struct Vertex
{
    ti::math::Vector3 pos;
    ti::math::Vector3 color;
    ti::math::Vector2 texCoord;
};

class VoxelLayer : public Layer
{
public:
    VoxelLayer()
    {
        Vertex vertices[]
        {
            {.pos = {-0.5f, -0.5f, -3.0f}, .color = {1.0f, 1.0f, 1.0f}, .texCoord = {0.0f, 0.0f}},
            {.pos = { 0.5f, -0.5f, -3.0f}, .color = {1.0f, 1.0f, 1.0f}, .texCoord = {1.0f, 0.0f}},
            {.pos = { 0.5f,  0.5f, -3.0f}, .color = {1.0f, 1.0f, 1.0f}, .texCoord = {1.0f, 1.0f}},
            {.pos = {-0.5f,  0.5f, -3.0f}, .color = {1.0f, 1.0f, 1.0f}, .texCoord = {0.0f, 1.0f}}
        };

        unsigned int indices[]
        {
            0, 1, 2,
            2, 3, 0
        };

        m_VertexBuffer = ti::graphics::ogl::CreateBuffer(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        m_IndexBuffer = ti::graphics::ogl::CreateBuffer(sizeof(indices), indices, GL_DYNAMIC_DRAW);
        
        glCreateVertexArrays(1, &m_VertexArray);
        glVertexArrayVertexBuffer(m_VertexArray, 0, m_VertexBuffer, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(m_VertexArray, m_IndexBuffer);

        glEnableVertexArrayAttrib(m_VertexArray, 0);
        glEnableVertexArrayAttrib(m_VertexArray, 1);
        glEnableVertexArrayAttrib(m_VertexArray, 2);

        glVertexArrayAttribFormat(m_VertexArray, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(m_VertexArray, 1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float));
        glVertexArrayAttribFormat(m_VertexArray, 2, 2, GL_FLOAT, GL_FALSE, (3+3)*sizeof(float));

        glVertexArrayAttribBinding(m_VertexArray, 0, 0);
        glVertexArrayAttribBinding(m_VertexArray, 1, 0);
        glVertexArrayAttribBinding(m_VertexArray, 2, 0);

        m_Program = ti::graphics::ogl::CreateProgram("tinkering/editor/shader.vert", "tinkering/editor/shader.frag");

        {
            const ti::math::Matrix4 view = ti::math::Matrix4::IDENTITY;
            glProgramUniformMatrix4fv(m_Program, glGetUniformLocation(m_Program, "uView"), 1, GL_FALSE, view.data);
        }
        {
            const ti::math::Matrix4 proj = ti::math::Matrix4::Perspective(ti::math::ToRadians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
            glProgramUniformMatrix4fv(m_Program, glGetUniformLocation(m_Program, "uProj"), 1, GL_FALSE, proj.data);
        }
    }

    virtual void Update(float dt) override
    {
        m_Position.x += 0.2f * dt;
        m_Position.z -= 0.8f * dt;
        m_Roll += 1.4f * dt;
        m_World = ti::math::Matrix4::IDENTITY;
        m_World.RotateZ(m_Roll);
        m_World.Translate(m_Position);
    }

    virtual void Render() override
    {
        glProgramUniformMatrix4fv(m_Program, glGetUniformLocation(m_Program, "uWorld"), 1, GL_FALSE, m_World.data);

        glUseProgram(m_Program);
        glBindVertexArray(m_VertexArray);
        glEnable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        glDisable(GL_DEPTH_TEST);
    }

private:
    ti::math::Vector3 m_Position = ti::math::Vector3::ZERO;
    ti::math::Matrix4 m_World = ti::math::Matrix4::IDENTITY;
    float m_Roll = 0.0f;

    GLuint m_VertexArray;
    GLuint m_VertexBuffer;
    GLuint m_IndexBuffer;
    GLuint m_Program;
};

int main()
{
    Application* app = new Application;
    Layer* voxelLayer = new VoxelLayer;
    app->PushLayer(voxelLayer);
    app->Run();
    delete app;
    delete voxelLayer;

    return 0;
}