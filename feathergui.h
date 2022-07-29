#ifndef FEATHERGUI
#define FEATHERGUI

#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>

#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ImageIO/stb_image.h"
#include "ImageIO/stb_image_write.h"

struct RGB {
	float r;
	float g;
	float b;
	float delta;
};

struct ImageStr {
	std::string imagePath;
	int width;
	int height;
	int channels;
	unsigned char* data;
	GLuint texture;
	bool loaded;
};

class FeatherGUI
{
	private:
		
		//MAIN WINDOW VARS
		bool isOpen;
		bool debugConsole;
		bool placementConfig;
		bool disableWindowsRounding;
		int windowWidth; //Pixels
		int windowHeight; //Pixels
		
		//FONTS AND TEXT VARS
		float iconSize; //Pixels

		//IMAGE VARS
		float zoomIncrement; //Percentage of zoom per click
		float zoom; // Percentage
		int imageShiftX; //Pixels
		int imageShiftY; //Pixels
		
		//GENERAL UI PLACEMENT VARS
		int toolsPanelPixels; //Pixels
		int propertiesPanelPixels; //Pixels
		int infoPanelPixels; //Pixels

		//IMGUI VARS
		RGB BackGroundRGB;
		std::vector<ImageStr> Images;
		ImageStr CurrentImage;
		ImFont* CurrentFont;
		
		int MouseImagePositionX; //Pixels
		int MouseImagePositionY; //Pixels

		const int MenuSizePixels = 22; //Pixels
		const int SeparatorSizePixels = 8; //Pixels

		//CONSOLE
		std::streambuf* coutbuff;
		std::ostringstream ss;
		
		//---------------------------METODS---------------------------
		void setIconSize(float _size);
		float getIconSize();
		
		bool loadImage(std::string _path);	
		
	public:
		
		GLFWwindow* windowContext;
		ImGuiIO* io;

		FeatherGUI(GLFWwindow* _windowContext, const char* _glsl_version);
		~FeatherGUI();
		
		bool IsOpened();

		void InitGUI();
		void BuildGUI();
		void RenderGUI();

		void SetBackGroundColor(float _r, float _g, float _b);
		RGB GetBackGroundColor();

		void centerImage();
};

#endif // !FEATHERGUI
