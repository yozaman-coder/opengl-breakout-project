#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include "window_functions.hpp"
#include "shader_s.hpp"
#include "stb/stb_image.h"
#include <filesystem>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

static std::filesystem::path getPath()
{
#ifdef  _WIN32
	//Get EXE path on windows
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return std::filesystem::path(buffer).parent_path().append("../../");
#elif defined(__linux__) || defined(__unix__)
	return std::filesystem::current_path().append("../");
#else
	std::cout << "Unknown OS" << std::endl;
	throw;
#endif
}

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

	Shader myShaders("../assets/shaders/shader.vertexShader", "../assets/shaders/shader.fragShader");

	float vertices[] = {
		// Positions		// Colors		// Texture Coordinates
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // Top-right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // Bottom-right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // Bottom-left
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f  // Top-left
	};

	unsigned int indices[] = {
		0, 1, 3, // First triangle
		1, 2, 3  // Second triangle
	};

	// Create and compile vertex shader.
	unsigned int VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Create buffers and store vertices in them
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position Data Location
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color Data Location
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Coordinate Data Location
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Tell open gl how to interpret the vertex data we have provided. GL_LINE for wireframe, GL_FILL for fill
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int textures[2];

	glGenTextures(2, &textures[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);

	std::string pathToTexture = getPath().append("assets/textures/container.jpg").string();

	std::cout << pathToTexture << std::endl;
	unsigned char* data = stbi_load(pathToTexture.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glGenTextures(1, &textures[1]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	pathToTexture = getPath().append("assets/textures/awesomeface.png").string();

	data = stbi_load(pathToTexture.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	myShaders.use();
	glUniform1i(glGetUniformLocation(myShaders.ID, "texture1"), 0);
	myShaders.setInt("texture2", 1);

	// Keep window open unless it should close
	while(!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands
		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		//float timeValue = glfwGetTime();
        	//float greenValue = (sin(timeValue * 5.0f) / 2.0f) + 0.5f;
		//float redValue = (sin(timeValue * 1.5f) / 2.0f) + 0.5f;
		//float blueValue = (sin(timeValue * 2.0f) / 2.0f) + 0.5f;

		//myShaders.set4Floats("gigaColor", redValue, greenValue, blueValue, 1.0f);
		
		myShaders.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_INT, 0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
