Tinker
------
Tinker was supposed to be an open-source game engine.

The keyword is "was supposed" because it is no longer maintained. <br/>
At the moment, I do not plan to continue it. <br/>
However, I learned so much while creating this scrappypiece! <br/>

Why is it no longer maintained?
-------------------------------
This was my personal decision based on many factors. <br/>
One of these factors is a more efficient use of my time (I love being efficient; why did I even start this project? :P).

Things to note
--------------
- I started creating this project using C++ guidelines, but over time I preferred C more. As a result, the code has mixed C++- and C-like code. <br/>
Why did this happen? <br/>
As I became more and more focused on code performance and after reading many articles and reviewing several books, I came across these great guys: Mike Acton (Insomniac Games), Casey Muratori (Handmade Hero) and Jonathan Blow (The Witness). <br/>
They all taught me many great things and gave me a different perspective on many things.

- This is not my first game engine development project. <br/>
However, this one was most focused on creating a game engine, not a game (don't do that - keep the game engine next to the game, because different games have different requirements, and the perfect game engine does not exist).

Why would you even touch this code?
-----------------------------------
Even if the code is very chaotic and some parts are not completed, I think you can at least:
- Inspire yourself,
- Learn more about CMake setup for more complex projects (e.g. separate source and library code),
- Learn how to render basic triangle in Vulkan (tinkering/vulkan_app/ directory; CAUTION: vertex buffer not included),
- Learn how to setup basic 3D camera in OpenGL (tinkering/opengl_app/ directory),
- See how wrapping OpenGL can look like (tinker/source/tinker/graphics/internal/ogl directory),
- See how math library can be implemented (tinker/source/tinker/math/ directory).

Lessons learned
---------------
- If you want to learn something, many small projects can yield better results (because you can prototype more easily and not worry about how changes will affect the project),
- KISS,
- Have a reason,
- Refactor it to keep it in good shape,
- Do not try to do it all yourself and instead use libraries (of course, you can always try to create a library yourself and then use it in your project),
- Plan, plan, plan, plan!
