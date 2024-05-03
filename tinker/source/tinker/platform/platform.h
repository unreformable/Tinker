#pragma once

#include "tinker/math/vector2.h"

#include <string>



// TODO: How to handle input? Needs: do smth on key click inside window, fetch for key states and mouse position PER WINDOW?????
// TODO: Monitor class???

namespace ti::platform
{
    void initalize();
    void terminate();
    
    class Window
    {
    public:
        // void focus();
        // void minimize();
        // void maximize();
        // void restore(); // restores window fram minimize or maximize
        // void hide(); // removes even from task bar etc.; cannot be hidden if is maximized
        // void show();
        // void requestAttention(); // not cross platform. should it be here then?

        void setPosition(const math::Vector2& position);
        const math::Vector2& getPosition() const;

        void setSize(const math::Vector2& position);
        const math::Vector2& getSize() const;

        void setTitle(const std::string& title);
        const std::string& getTitle() const;

        void setVisible(bool value);
        bool isVisible() const;

        void* getNativeHandle() const;
    
    private:
        math::Vector2 m_Position;
        math::Vector2 m_Size;
        std::string m_Title;
        bool m_Visible;
    };
}

// bool isKeyPressed(Key key);
// bool isMouseButtonPressed(MouseButton button);

// math::Vector2 getMousePosition();
// math::Vector2 getMouseScrollDelta();