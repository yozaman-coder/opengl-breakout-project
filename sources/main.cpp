#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include "window_functions.h"

float vertices[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

float vertices2[] = {		//colors
	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	
	-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f
};	//positions

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"out vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
	"}\0";

const char *vertexShaderSource2 = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 newColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"	newColor = aColor;\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = ourColor;\n"
	"}\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 newColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(newColor, 1.0);\n"
	"}\0";

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Balls", NULL, NULL);

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
	unsigned int vertexShader;
	unsigned int vertexShader2;
	unsigned int fragmentShader;
	unsigned int fragmentShader2;
	unsigned int shaderProgram;
	unsigned int shaderProgram2;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int VBO2;
	unsigned int VAO2;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);

	// Create buffers and store vertices in them
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	shaderProgram = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Link the shaders we created to the shader program.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glGenBuffers(1, &VBO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	shaderProgram2 = glCreateProgram();
	vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader2, 1, &vertexShaderSource2, NULL);
	glCompileShader(vertexShader2);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	glAttachShader(shaderProgram2, vertexShader2);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	// Was vertex shader compilation successful?
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::printf("%s\n %s\n", "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", infoLog);
	}

	glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader2, 512, NULL, infoLog);
		std::printf("%s\n %s\n", "ERROR::SHADER::VERTEX::COMPILATION_FAILED2\n", infoLog);
	}

	// Was fragment shader compilation successful?
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::printf("%s\n %s\n", "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog);
	}
	
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::printf("%s\n %s\n", "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED2\n", infoLog);
	}

	// Was the shader program creation and linkage successfull?
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::printf("%s\n %s\n", "ERROR::SHADER::PROGRAM::LINK_FAILED\n", infoLog);
	}

	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::printf("%s\n %s\n", "ERROR::SHADER::PROGRAM::LINK_FAILED2\n", infoLog);
	}

	// Delete shaders now that we have the shader program, we don't need them.
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShader2);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);

	// Tell open gl how to interpret the vertex data we have provided.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Keep window open unless it should close
	while(!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands
		glClearColor(0.0f, 0.0f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw green triangle
		glUseProgram(shaderProgram);

		float timeValue = glfwGetTime();
        	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Draw rainbow triangle
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
