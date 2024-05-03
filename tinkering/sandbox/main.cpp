#include "tinker/core/debug.h"
#include "tinker/core/file.h"
#include "tinker/core/image.h"
#include "tinker/math/matrix4.h"
#include "tinker/math/trigonometry.h"
#include "tinker/graphics/ogl/ogl.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>



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
        GE_ASSERT(false, "");
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
        GE_ASSERT(false, "");
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
        GE_ASSERT(false, "");
        return "";
	}();
	std::cerr << sourceStr << ", " << typeStr << ", " << severityStr << ", " << id << ": " << message << '\n';
}

struct Vertex
{
    math::Vector3 pos;
    math::Vector3 color;
    float uv[2];
};

static int keys[GLFW_KEY_LAST]{};

int main()
{
    glfwSetErrorCallback(glfwErrorCallback);

    if(!glfwInit())
        return 1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 450, "Window title here", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglErrorCallback, nullptr);
    //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    const Vertex vertices[]{
        {{-0.5f, 0, -0.5f}, {0.5f, 0.5f, 0.5f}, {0, 0}},
        {{ 0.5f, 0, -0.5f}, {0.5f, 0.5f, 0.5f}, {1, 0}},
        {{ 0.5f, 0,  0.5f}, {0.5f, 0.5f, 0.5f}, {1, 1}},
        {{-0.5f, 0,  0.5f}, {0.5f, 0.5f, 0.5f}, {0, 1}}
    };
    const GLuint vbo = ogl::CreateBuffer(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    const unsigned int indices[]{
        0, 1, 2,
        2, 3, 0
    };
    const GLuint ibo = ogl::CreateBuffer(sizeof(indices), indices, GL_DYNAMIC_DRAW);

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(vao, ibo);

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float));
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, (3+3)*sizeof(float));

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribBinding(vao, 2, 0);

    GLuint program = ogl::CreateProgram("../../../examples/sandbox/shader.vert", "../../../examples/sandbox/shader.frag");

    u32 width;
    u32 height;
    u32 channels;
    auto pixels = readImage("../../../examples/sandbox/image.jpg", width, height, channels);

    GLuint texture = ogl::CreateTexture2D(pixels, width, height, 1, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
    glBindTextureUnit(0, texture);

    freeImage(pixels);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        
        if(action == GLFW_PRESS)   keys[key] = 1;
        if(action == GLFW_RELEASE) keys[key] = 0;
    });

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    math::Vector3 cameraPos = -4 * math::Vector3::FORWARD;
    math::Vector3 target = math::Vector3::FORWARD;

    math::Matrix4 world = math::Matrix4::IDENTITY;
    world.translate({2, 2, 1}).rotateX(math::toRadians(15.0f)).rotateZ(math::toRadians(45.0f));
    const math::Matrix4 proj = math::Matrix4::perspective(math::toRadians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
    //const math::Matrix4 proj = math::Matrix4::orthographic(-1.6f, 1.6f, -0.9f, 0.9f, 0.1f, 20.0f);
    glProgramUniformMatrix4fv(program, glGetUniformLocation(program, "uWorld"), 1, GL_FALSE, world.data);
    glProgramUniformMatrix4fv(program, glGetUniformLocation(program, "uProj"), 1, GL_FALSE, proj.data);

    // glm::mat4 w = glm::mat4(1.0f);
    // w = glm::translate(w, {2, 2, 1});
    // const glm::mat4 v = glm::lookAtRH(glm::vec3(0, -4, 0), {0, 1, 0}, {0, 0, 1});
    // const glm::mat4 p = glm::perspectiveRH(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
    // glProgramUniformMatrix4fv(program, glGetUniformLocation(program, "uWorld"), 1, GL_FALSE, &w[0][0]);
    // glProgramUniformMatrix4fv(program, glGetUniformLocation(program, "uView"), 1, GL_FALSE, &v[0][0]);
    // glProgramUniformMatrix4fv(program, glGetUniformLocation(program, "uProj"), 1, GL_FALSE, &p[0][0]);

    float delta = 0.0f;
    double last = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if(keys[GLFW_KEY_W]) cameraPos.y += 1.0f * delta;
        if(keys[GLFW_KEY_S]) cameraPos.y -= 1.0f * delta;
        if(keys[GLFW_KEY_D]) cameraPos.x += 1.0f * delta;
        if(keys[GLFW_KEY_A]) cameraPos.x -= 1.0f * delta;
        if(keys[GLFW_KEY_SPACE]) cameraPos.z += 1.0f * delta;
        if(keys[GLFW_KEY_LEFT_SHIFT]) cameraPos.z -= 1.0f * delta;

        math::Matrix4 view = math::Matrix4::lookAt(cameraPos, target);
        glProgramUniformMatrix4fv(program, glGetUniformLocation(program, "uView"), 1, GL_FALSE, view.data);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glEnable(GL_DEPTH_TEST);
        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDisable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);

        const double current = glfwGetTime();
        delta = (float)(current - last);
        last = current;

        static float timer = 0.0f;
        static int fps = 0;
        timer += delta;
        fps++;
        if(timer >= 1.0f)
        {
            const std::string newTitle = "Window title here [FPS:" + std::to_string(fps) + ']';
            glfwSetWindowTitle(window, newTitle.c_str());
            timer -= 1.0f;
            fps = 0;
        }
    }
    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}