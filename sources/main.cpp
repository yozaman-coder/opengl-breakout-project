#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include "window_functions.h"

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

	glViewport(0, 0, 800, 600);

	// When window is resized set the frame buffer size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Keep window open unless it should close
	while(!glfwWindowShouldClose(window))
	{
		// Swaps front and back "buffers" so that you don't see the window being rendered.
		// Basically renders the view in the background, then when it's ready swaps it in
		// so the user does not see the window being loaded.
		glfwSwapBuffers(window);

		// Looks for if any events are triggered, such as keyboard/mouse movement, window state, or callback methods.
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
