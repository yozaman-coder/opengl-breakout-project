#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include "window_functions.hpp"
#include "shader_s.hpp"

// Upside down triangle
float vertices[] = {
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f,
};	//positions

int main()
{
	// Initialize GLFW window creator thing
	glfwInit();
	// Specify what opengl version in this case we are using 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use the CORE profile which is a smaller subset of OpenGL features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window object called "Balls"
	GLFWwindow* window = glfwCreateWindow(800, 600, "Breakout", NULL, NULL);

	if (window == NULL)
	{
		std::println("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Load glad and pass a function that loads the OpenGL function pointers, using glfwGetProcAddress to get the function based on
	// the current OS
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::println("Failed to intialize GLAD");
		return -1;
	}

	int fbSizeX, fbSizeY;
	glfwGetFramebufferSize(window, &fbSizeX, &fbSizeY);
	glViewport(0, 0, fbSizeX, fbSizeY);

	// When window is resized set the frame buffer size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Create and compile vertex shader.
	unsigned int VBO;
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);

	// Create buffers and store vertices in them
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Shader myShaders("../assets/shader.vertexShader", "../assets/shader.fragShader");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Tell open gl how to interpret the vertex data we have provided.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Keep window open unless it should close
	while(!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands
		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShaders.use();
		float timeValue = glfwGetTime();
        	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		myShaders.set4Floats("gigaColor", 0.0f, greenValue, 0.0f, 1.0f);
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
