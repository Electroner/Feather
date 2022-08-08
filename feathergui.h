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
	std::string extension;
	std::string name;
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
		bool disableOptionsRounding;
		int windowWidth; //Pixels
		int windowHeight; //Pixels
		
		//FONTS AND TEXT VARS
		float iconSize; //Pixels

		//IMAGE VARS
		float zoomIncrement; //Percentage of zoom per click
		float zoom; // Percentage
		float maxZoom; //Absolut ezoom level
		float minZoom; //Absolut ezoom level
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

		//ICON IMAGES
		std::vector<ImageStr> toolsIcons;
		ImVec4 colorNoSelectedMenu;
		ImVec4 colorSelectedMenu;
		
		int MouseImagePositionX; //Pixels
		int MouseImagePositionY; //Pixels

		const int MenuSizePixels = 22; //Pixels
		const int SeparatorSizePixels = 8; //Pixels

		float windowRounding;

		//OPTIONS VARS
		bool Vsync;

		//TOOLS VARS
		int CurrentTool;

		//CONSOLE
		std::streambuf* coutbuff;
		std::ostringstream ss;
		
		//---------------------------METHODS---------------------------
		void setIconSize(float _size);
		float getIconSize();
		
		bool loadImage(std::string _path);	
		bool loadIcon(std::string _path);

		//---------------------------OPENGL---------------------------
		void SetSync(bool _sync);
		bool GetSync();
		
	public:
		
		GLFWwindow* windowContext;
		ImGuiIO* io;

		FeatherGUI(GLFWwindow* _windowContext, const char* _glsl_version);
		~FeatherGUI();

		void HelpMarker(const char* desc);
		
		bool IsOpened();

		void InitGUI();
		void BuildGUI();
		void RenderGUI();

		void SetBackGroundColor(float _r, float _g, float _b);
		RGB GetBackGroundColor();
		
		void calculateZoom();
		void centerImage();

		//STATIC MENUS
		void BuildMenu();
		void BuildTools();
		void BuildImageDisplayer();
		void BuildProperties();
		void BuildLayers();
		void BuildInfo();
		
		//DYNAMIC MENUS
		void BuildConfigMenu();
		void BuildConsoleDebugMenu();

		//INPUT FUNCTIONS
		void InputFunctions();
};

#endif // !FEATHERGUI

