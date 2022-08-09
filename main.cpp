#include "pathwork.h"
#include "feathergui.h"

#define ICONPATH "/Images/Feather.png"

//#define DEBUG

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	glfwSetErrorCallback(glfw_error_callback);
	GLFWwindow* window;

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	if (!glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(1280, 720, "", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);	// Enable vsync

	//Icon Path and Load image 
	std::string icon_path = getNormalizedPath() + ICONPATH;
	int icon_x, icon_y, icon_nrChannels;
	unsigned char* icon_pixels = stbi_load(icon_path.c_str(), &icon_x, &icon_y, &icon_nrChannels, 4);
	//Create GLFW Image
	GLFWimage GLicon[1];
	GLicon[0].pixels = icon_pixels;
	GLicon[0].width = icon_x;
	GLicon[0].height = icon_y;
	//change window icon
	glfwSetWindowIcon(window, 1, GLicon);

	//Set title of the window
	glfwSetWindowTitle(window, "Feather");

	//Make it fullscreen
	//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_DONT_CARE);

	//Make the windows maximized
	glfwMaximizeWindow(window);

	//Create the GUI
	FeatherGUI gui(window, glsl_version);

	while (!glfwWindowShouldClose(window) && gui.IsOpened())
	{
		glfwPollEvents();
		
		gui.InitGUI();
		gui.BuildGUI();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(gui.GetBackGroundColor().r, gui.GetBackGroundColor().g, gui.GetBackGroundColor().b, gui.GetBackGroundColor().delta);
		glClear(GL_COLOR_BUFFER_BIT);
		
		gui.RenderGUI();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}