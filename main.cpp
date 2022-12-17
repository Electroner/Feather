#include "./GUI/feathergui.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//#define DEBUG

#ifndef DEBUG

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
	glfwSwapInterval(0);	// Enable vsync

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
		glClearColor(
			(GLclampf)(gui.GetBackGroundColor().r / 255.0F),
			(GLclampf)(gui.GetBackGroundColor().g / 255.0F),
			(GLclampf)(gui.GetBackGroundColor().b / 255.0F),
			(GLclampf)(gui.GetBackGroundColor().delta / 255.0F));
		glClear(GL_COLOR_BUFFER_BIT);
		
		gui.RenderGUI();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#endif

#ifdef DEBUG

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
	glfwSwapInterval(0);	// Enable vsync

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
	glfwSetWindowTitle(window, "TEST");

	//Make it fullscreen
	//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_DONT_CARE);

	//Make the windows maximized
	glfwMaximizeWindow(window);

	ImGui::CreateContext();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//Create a ImTextureID
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_x, icon_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, icon_pixels);
	
	int mouse_Size = 16;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		//TEST
		ImGui::Begin("TEST");
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		//Add a cursor using GetForegroundDrawList
		//ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetMousePos(), 10, IM_COL32(255, 0, 0, 255));
		//Render the icon into the mouse cursor shifted by 10 pixels
		ImGui::GetForegroundDrawList()->AddImage((ImTextureID)texture, 
			ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y + mouse_Size),
			ImVec2(ImGui::GetMousePos().x + mouse_Size, ImGui::GetMousePos().y));
		ImGui::End();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#endif