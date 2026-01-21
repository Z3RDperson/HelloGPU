#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader_master.h" // Shader header file that reads shaders from disk, compiles and links them & checks for errors

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Settings:
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


int main()
{
	glfwInit(); //Start GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Instantiate a GLFW window:
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL); //This function returns a pointer to a new window
	if (window == NULL) //Check if it failed
	{
		std::cout << "GLFW: Failed to create the window ! .. terminating" << std::endl;
		glfwTerminate(); //Stop GLFW
		return -1;
	}
	//Here we tell GLFW to make the context of our window the main context on the current thread, as successfully creating the window doesn't change the context
	glfwMakeContextCurrent(window);

	//GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function:
	//Here we ask GLAD to load (Load GL Loader) the address of the OpenGL function pointers (proc) which is OS-specific
	//And then ask GLFW for the address of that OS-specific proc to pass it to GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD: Failed to initialize GLAD ! .. terminating" << std::endl; //Check if it failed
		return -1;
	}


	//After GLFW created a window for our game and GLAD specified exact OpenGL version, we have to tell OpenGL the size of the rendering window
	//So OpenGL knows how we want to display the data and coordinates with respect to the window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); //Meaning before rendering we tell OpenGL : Render in a viewport of SCREEN_WIDTH x SCREEN_HEIGHT

	//We register the callback functions after we've created the window and before the render loop is initiated:
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Resize the render viewport whenever user resizes GLFW window by registering the callback we wrote to our GLFW window



	//Build & Compile a shader program:
	Shader RGBHelloTriangle("Shaders/vertexShader/RGB_HelloTriangle_vertexSh.glsl", "Shaders/fragmentShader/RGB_HelloTriangle_fragSh.glsl");



	//To start drawing something (Start the graphics pipeline) we have to first give OpenGL some input vertex data after processing it:
	//Step_1:Vertex data definition:
	float GLTriangles_Vertices[] = {
		// Position attribute:    Color toward vertex:   EBO index:   Current Shape:
		//X:   Y:    Z:           R:    G:    B:
	    0.5f, -0.5f, 0.0f,        1.0f, 0.0f, 0.0f,   // (0) Right    Colorful triangle
	   -0.5f, -0.5f, 0.0f,        0.0f, 1.0f, 0.0f,   // (1) Left
	    0.0f,  0.5f, 0.0f,        0.0f, 0.0f, 1.0f    // (2) Top
	};
	//EBO is buffer for storing indices of vertices we have (each unique vertex has an index) for index drawing
	unsigned int eboIndices[] = {
		0, 1, 2
	};

	//Step_2:Generating buffer objects with a buffer ID for each one:
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Step_3:Binding VAO first that will store upcoming-to-be-bound:
	//-----> 1) Vertex attribute configurations via glVertexAttribPointer & Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer
	//-----> 2) EBO (if VAO is unbound firstly)
	glBindVertexArray(VAO);

	//Step_4:Binding every generated buffer with the target suitable for its type
	//Step_5:Copy pre-defined data into the currently bound (allocated) buffer to store this data on GPU VRAM
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //We chose GL_ARRAY_BUFFER to indicate that we are binding a vertex buffer (its type basically which is an array of values back to back)
	//From that point on any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, which is VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLTriangles_Vertices), GLTriangles_Vertices, GL_STATIC_DRAW); //This function starts passing data into the VRAM buffer
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //We chose GL_ELEMENT_ARRAY_BUFFER this time to indicate that we are binding an element buffer (its type basically which is an array of elements (indices) back to back)
	//From that point on any buffer calls we make (on the GL_ELEMENT_ARRAY_BUFFER target) will be used to configure the currently bound buffer, which is EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eboIndices), eboIndices, GL_STATIC_DRAW);

	//Step_6:Interpreting vertex data by specifying each single vertex and each single attribute of this single vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //Color attribute data (2nd attribute)
	glEnableVertexAttribArray(1);

	//Step_7:Unbind buffers to free VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// REMEMBER!!: Do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound
	glBindVertexArray(0);



	//Create RENDER LOOP, that keeps running until we tell GLFW to explicitly stop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window); //Handle user input

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Rendering Commands <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Choose a color to replace color buffer contents with it
		glClear(GL_COLOR_BUFFER_BIT); //Clear only color buffer and replace it with the registered color

		RGBHelloTriangle.use();

		glBindVertexArray(VAO); //Bind the VAO of our GLTriangles vertex data so it knows automatically which VBO to process for its specific attribute config

		//glDrawArrays(GL_TRIANGLES, 0, 3); //Draw primitives using the previously defined vertex attribute configuration and with the VBO's vertex data (indirectly bound via the VAO)

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0); //Draw primitives using indices provided in the element buffer object (EBO that's currently bound automatically by VAO)

		//---------------------------------------------------------------------------------------------------------------------------------------------

		glfwSwapBuffers(window); //Swap the Double buffer to the back one when we end drawing the frame
		glfwPollEvents(); //Wait for any events from user (handled after that by callback functions)
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) //Callback definition to resize the render viewport whenever user resizes GLFW window
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) //Callback definition to handle user input in a specific window
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true); //If we pressed ESCAPE window should close
	}
}