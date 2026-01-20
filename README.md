# HelloGPU

My OpenGL learning project.

<img align="right" src="Documentation images/Screenshot-HelloTriangle-001.png" width="320px"/>

As I am still learning (both OpenGL & Git), I like to add many comments around code and leave old code commented out to point out the structure for myself and make a reference for future projects. So forgive me for lack of professionalism.

### How to build for Windows 10+ :
If you want to clone the project on your PC, you need: Visual Studio 2022 / 2026, GLFW library, GLAD library

Then do as following:

- Open the `HelloGPU.sln` solution file with Visual Studio.
- Set up third party libs (GLFW & GLAD) <b>for OpenGL 3.3 (Please be careful .. 3.3)</b> and don't forget to link their locations in project properties directories ( follow the tutorial on [learnopengl.com](https://learnopengl.com/Getting-started/Creating-a-window) )
- After setting up the libs, double click the solution file in solution explorer and wait for it to build
- If you found redlines or undefined code, it means that you didn't include third party libs properly
