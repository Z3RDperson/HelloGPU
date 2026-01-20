# HelloGPU

My OpenGL learning project.

<img align="right" src="Documentation images/Screenshot-HelloTriangle-001.png" width="320px"/>

As I am still learning (both OpenGL & Git), I like to add many comments around code and leave old code commented out to point out the structure for myself and make a reference for future projects. So forgive me for lack of professionalism.

### How to build for Windows 10+ :
If you want to build the project on your PC, you need: 
- Visual Studio 2022/26
- GLFW library
- GLAD library for OpenGL 3.3 core.

Then do as following:

- Open the `HelloGPU.sln` solution file with Visual Studio and build the solution or click "Clone a repository" in get started window to clone it on your disk and double click the solution file in solution explorer.
- After building the project files, Set up third party libs (GLFW & GLAD) <b>for OpenGL 3.3 with core profile (Please be careful .. 3.3 core)</b> and don't forget to link their locations in project properties directories ( follow this [tutorial](https://learnopengl.com/Getting-started/Creating-a-window) ).
- Now you can compile the code in `Main.cpp` and run the project.
- If you still found redlines or undefined code, it means that you didn't include third party libs properly. Make sure you followed the tutorial and chose the correct specification of GLAD.

### Backup .txt files :
Before getting introduced to Git, I played around with rendering code to try different things and stored it in these files. It's like a mini rudimentary version control for just the first RGB Hello Triangle, as it's the first and hardest milestone in learning any graphics API anyway.
