#include "tinker/platform/platform.h"



// by default each window is created inside primary monitor
// window.setMonitor() can be used to change used monitor by this window

int main()
{
    ti::platform::initalize();

    const ti::platform::Monitor* monitors = ti::platform::getMonitors();
    ti::platform::Window window;
    window.setPosition(monitors[0].size()/2 - window.size()/2);
    window.setSize({800, 450});
    window.setTitle("Game");
    window.setMonitor(monitors[0]);
    window.setVisible(true);

    platform::WindowLayout layout;
    window.setLayout(layout);

    while(true)
    {
        ti::platform::pollEvents();

        //window.update(); ????
    }

    ti::platform::terminate();
}