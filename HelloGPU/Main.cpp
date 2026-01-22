#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <iostream>

#include "shader_master.h" // Shader header file that reads shaders from disk, compiles and links them & checks for errors

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Settings:
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Just for textures blending:
float MIX_INTENSITY = 0.2f;


int main()
{
	glfwInit(); //Start GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Instantiate a GLFW window:
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello GPU", NULL, NULL); //This function returns a pointer to a new window
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
	Shader myShader("Shaders/vertexShader/RGB_HelloTriangle_vertexSh.glsl", "Shaders/fragmentShader/RGB_HelloTriangle_fragSh.glsl");



	//To start drawing something (Start the graphics pipeline) we have to first give OpenGL some input vertex data after processing it:
	//Step_1:Vertex data definition:
	float GLTriangles_Vertices[] = {
		// Position attribute:    Color toward vertex:   texture[0] Coords:   texture[1] Coords:     EBO index:
		//X:   Y:    Z:           R:    G:    B:         S:     T:             S:     T:
	    0.5f,  0.5f, 0.0f,        1.0f, 0.0f, 0.0f,      1.0f,  1.0f,          1.0f,  1.0f,          // (0) Right Top
	    0.5f, -0.5f, 0.0f,        0.0f, 1.0f, 0.0f,      1.0f,  0.0f,          1.0f,  0.0f,          // (1) Right Bottom
	   -0.5f, -0.5f, 0.0f,        0.0f, 0.0f, 1.0f,      0.0f,  0.0f,          0.0f,  0.0f,          // (2) Left Bottom
	   -0.5f,  0.5f, 0.0f,        1.0f, 0.0f, 1.0f,      0.0f,  1.0f,          0.0f,  1.0f           // (3) Left Top
	};
	//EBO is buffer for storing indices of vertices we have (each unique vertex has an index) for index drawing
	unsigned int eboIndices[] = {
		0, 1, 3,
		1, 2, 3
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0); //Position attribute data (1st attribute)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float))); //Color attribute data (2nd attribute)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float))); //Texture[0] Coords attribute data (3rd attribute)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float))); //Texture[1] Coords attribute data (3rd attribute)
	glEnableVertexAttribArray(3);

	//Step_7:Unbind buffers to free VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// REMEMBER!!: Do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound
	glBindVertexArray(0);

	// Optional for now: Now you can unbind EBO



	// Texture work :
	
	// Generate objects for our textures as usual:
	unsigned int textures[2];
	glGenTextures(2, textures);
	
	// Load the first texture (texture[0]):
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	// Texture is a 2D image wrapped on our geometry. To know which part of texture will be visible on our geometry, we must sample textures
	// Sampling means specifying where vertices will be on a normalized range of texture plane

	// Set texture wrapping mode :
	// XYZ of texture is : str
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Arguments: OpenGL state machine target that texture will be bound to (in this case 2D bcs we want to wrap a 2d image) - texture option (in this case wrapping on the 2D axes) - wrapping mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// If we chose GL_CLAMP_TO_BORDER wrapping, we should also specify a border color
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Set texture filtering :
	// Texture coordinates do not depend on resolution as texture resolution can vary if object size is the same
	// texture may be small and object is very large or vice versa so to fix distorted resolution we use texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Linear filtering + Mipmapping .. Put on minifying to specify the transition between mipmaps when getting far from texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR: produces a smoother pattern, GL_NEAREST: results in blocked patterns .. Put on magnifying to specify the filtering when stretching of texture resolution

	// If images loaded flipped:
	stbi_set_flip_vertically_on_load(true);

	// Load a 2D image using stb_image.h function into a string variable:
	int width, height, colorChNum;
	unsigned char *image0_Data = stbi_load("Textures/images/island.png", &width, &height, &colorChNum, 4);

	// Check if image failed to load and pointer is NULL:
	if (image0_Data)
	{
		// Load the 2D image into current object and generate a texture out of it:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image0_Data);
		// Arguments:   Target,   mipmap,img format, size,   ??, format and type of data and a pointer to data itself of the image

		// Set mipmap :
		// Mipmaps are exactly like LODs but specifically for textures. Just as a Mesh LOD reduces the triangles for a distant model, a mipmap reduces the resolution of a texture for a distant object
		// Mipmap is basically a collection of texture images where each subsequent texture is twice as small compared to the previous one
		glGenerateMipmap(GL_TEXTURE_2D); // Automatically generate a mipmap with image halved until its resolution is 1 pixel. If you want to avoid that just specify how many divisions you need in glTexImage2D()
	}
	else
	{
		std::cout << "Failed to load the texture" << std::endl;
	}

	// Free VRAM of texture:
	stbi_image_free(image0_Data);

	// Load the second texture (texture[1]):
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image1_Data = stbi_load("Textures/images/kenway.png", &width, &height, &colorChNum, 4);
	if (image1_Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1_Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load the texture" << std::endl;
	}
	stbi_image_free(image1_Data);

	// Compile the shader program to reach the uniforms of samplers inside it:
	myShader.use();
	glUniform1i(glGetUniformLocation(myShader.ID, "myTexture_0"), 0); // Get the uniform called myTexture_0 form our shader program and pass to it the texture UNIT (value of sampler uniform)
	glUniform1i(glGetUniformLocation(myShader.ID, "myTexture_1"), 1); // Get the uniform called myTexture_0 form our shader program and pass to it the texture UNIT (value of sampler uniform)



	// RENDER LOOP :
	while (!glfwWindowShouldClose(window))
	{
		// Handle user input
		processInput(window);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Rendering Commands <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Choose a color to replace color buffer contents with it
		glClear(GL_COLOR_BUFFER_BIT); // Clear only color buffer and replace it with the registered color

		// Activate and Bind textures:
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		glUniform1f(glGetUniformLocation(myShader.ID, "mix_intensity"), MIX_INTENSITY);

		// Use currently bound shader program
		myShader.use();

		// Bind the VAO of our GLTriangles vertex data so it knows automatically which VBO to process for its specific attribute config
		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLES, 0, 3); //Draw primitives using the previously defined vertex attribute configuration and with the VBO's vertex data (indirectly bound via the VAO)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Draw primitives using indices provided in the element buffer object (EBO that's currently bound automatically by VAO)

		//---------------------------------------------------------------------------------------------------------------------------------------------

		glfwSwapBuffers(window); //Swap the Double buffer to the back one when we end drawing the frame
		glfwPollEvents(); //Wait for any events from user (handled after that by callback functions)
	}

	// Deallocate VRAM at the end:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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
		glfwSetWindowShouldClose(window, true); //If we pressed ESCAPE window should close

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (MIX_INTENSITY <= 1.0f)
		{
			MIX_INTENSITY += 0.1f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (MIX_INTENSITY >= 0.1f)
		{
			MIX_INTENSITY -= 0.1f;
		}
	}
}