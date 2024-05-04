#include "tinker/platform/platform.h"

#include "tinker/core/debug.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"



namespace ti::platform
{
    static void myGlfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "GLFW_ERROR: %s\n", description);
    }

    void initalize()
    {
        TI_ERROR("Make glfwSetErrorCallback interact with logger");
#ifndef NDEBUG
        glfwSetErrorCallback(myGlfwErrorCallback);
#endif
        TI_ASSERT(glfwInit() == GLFW_TRUE, "Could not initalize glfw");

        TI_ERROR("Improve glfwWindowHint so they change depending on platform used (platform detection needed!)");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void terminate()
    {
        glfwTerminate();
    }
    
    // class Window
    // {
    // public:
    //     // void focus();
    //     // void minimize();
    //     // void maximize();
    //     // void restore(); // restores window fram minimize or maximize
    //     // void hide(); // removes even from task bar etc.; cannot be hidden if is maximized
    //     // void show();
    //     // void requestAttention(); // not cross platform. should it be here then?

    //     void setPosition(const math::Vector2& position);
    //     const math::Vector2& getPosition() const;

    //     void setSize(const math::Vector2& position);
    //     const math::Vector2& getSize() const;

    //     void setTitle(const std::string& title);
    //     const std::string& getTitle() const;

    //     void setVisible(bool value);
    //     bool isVisible() const;

    //     void* getNativeHandle() const;
    
    // private:
    //     math::Vector2 m_Position;
    //     math::Vector2 m_Size;
    //     std::string m_Title;
    //     bool m_Visible;
    // };
}