#ifndef FEATHERGUI
#define FEATHERGUI

#define ICONPATH "/resources/Images/Feather.png"

//#define DEBUG

#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <utility>

#define WINDOWS_SO
//#define LINUX_SO

#ifdef WINDOWS_SO
#include <windows.h>
#include <shlobj_core.h>
#include <commdlg.h>
#endif 

#ifdef LINUX
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../imgui/implot.h"

#include "../ImageIO/stb_image.h"
#include "../ImageIO/stb_image_write.h"

#include "../ImageWork/ImageWork.h"

#include "../Directories/pathwork.h"
#include "IconsFontAwesome.h"

class FeatherGUI
{
	private:
		
		//MAIN WINDOW VARS
		bool isOpen;
		int windowWidth; //Pixels
		int windowHeight; //Pixels
		bool CursorEnabled;
		
		bool placementConfig; //Extern Window
		bool debugConsole; //Debug console | Extern Window
		bool newImagePopUp; //Extern Window
		bool histogramPopUp;

		//MAIN WINDOWS CONFIRGURATION VARS
		bool disableOptionsRounding; //Option for rounding
		
		//ERROR WINDOW VARS
		int errorWindowCode; //Extern Window
		
		//FONTS AND TEXT VARS
		float iconSize; //Pixels
		std::string fontsPath;

		//IMAGE VARS
		bool MouseOverImageWindow;
		float zoomIncrement; //Percentage of zoom per click
		float zoom; // Percentage
		float maxZoom; //Absolut ezoom level
		float minZoom; //Absolut ezoom level
		int imageShiftX; //Pixels
		int imageShiftY; //Pixels
		int newImageWidth;
		int newImageHeight;
		char newImageName[100] = "New Image";
		bool newImageTransparency;
		
		//GENERAL UI PLACEMENT VARS
		int toolsPanelPixels; //Pixels
		int propertiesPanelPixels; //Pixels
		int infoPanelPixels; //Pixels

		//IMGUI VARS
		ImageWork workStation;
		RGB BackGroundRGB;
		ImFont* CurrentFont;

		//ICON IMAGES
		std::vector<ImageStr> toolsIcons;
		
		//CURSORS IMAGES
		std::vector<ImageStr> cursors;

		//IMAGES DROP EVENT
		static std::vector<std::string> droppedFiles;

		//COLORS
		bool whiteIcons;
		ImVec4 colorNoSelectedTool;
		ImVec4 colorSelectedTool;
		ImVec4 colorWindowBar;
		ImVec4 colorSelectedWindow;
		ImVec4 colorNoSelectedWindow;
		ImVec4 SliderGrab;
		ImVec4 SliderGrabActive;
		ImVec4 ActuatorsBackground;
		
		int MouseImagePositionX; //Pixels
		int MouseImagePositionY; //Pixels

		const int MenuSizePixels = 22; //Pixels
		const int SeparatorSizePixels = 8; //Pixels

		float windowRounding;

		//OPTIONS VARS
		bool Vsync;

		//TOOLS VARS
		int CurrentTool;
		bool ToolPrevisualization;
		
		//Selection Indexs
		int selectionSquareSize;
		float indexMinX; //pixel where the selection starts
		float indexMinY; //pixel where the selection starts
		float indexMaxX; //pixel where the selection ends
		float indexMaxY; //pixel where the selection ends
		//Selection FixMouse
		bool topLeft;
		bool topRight;
		bool bottomLeft;
		bool bottomRight;
		bool middleLeft;
		bool middleRight;
		bool middleTop;
		bool middleBottom;
		bool middle;

		//Selection Scale
		int selectionScaleSquareSize;
		float indexScaleMinX; //pixel where the selection starts
		float indexScaleMinY; //pixel where the selection starts
		float indexScaleMaxX; //pixel where the selection ends
		float indexScaleMaxY; //pixel where the selection ends
		//Selection FixMouse
		bool topLeftScale;
		bool topRightScale;
		bool bottomLeftScale;
		bool bottomRightScale;
		
		//CONSOLE
		std::streambuf* coutbuff;
		std::ostringstream ss;
		
		//---------------------------METHODS---------------------------
		void setIconSize(float _size);
		float getIconSize();
		
		bool loadImage(std::string _path);
		bool loadIcon(std::string _path);
		bool loadCursor(std::string _path);
		bool loadFromClipBoard();
		bool saveImage(std::string _path);
		bool copySelectionToClipboard();
		bool copyImageToClipboard();

		static void drop_callback(GLFWwindow* _windowContext, int _count, const char** _paths);
		bool loadDroppedImages();

		//---------------------------OPENGL---------------------------
		void SetSync(bool _sync);
		bool GetSync();
		
		//---------------------------IMGUI---------------------------
		void HelpMarker(const char* desc);
		void FloatingText(char* desc);
		void FloatingText(const char* desc);

		void calculateZoom();
		void centerImage();
		void UpdateImage();

		//STATIC MENUS
		void BuildMenu();
		void BuildTools();
		void BuildImageDisplayer();
		void BuildProperties();
		void BuildLayers();
		void BuildInfo();

		//MENU FUNCTIONS
		void OpenMenuFunction();
		void SaveMenuFunction(int _type); // 0 = Save | 1 = Save As

		//ERROR WINDOWS
		void ErrorWindow(); // Error creating a Image (Associated to errorWindowCreateImage bool )

		//OPTIONS RELATED FUNCTIONS
		void BuildToolProperties();
		void toolDisplays();
		void selectionUpdate();

		//DYNAMIC MENUS
		void BuildConfigMenu();
		void BuildConsoleDebugMenu();
		void BuildHistogram();

		//INPUT FUNCTIONS
		void InputFunctions();

		//IMAGE RELATED FUNCTIONS
		void newImage();
		
	public:
		
		GLFWwindow* windowContext;
		ImGuiIO* io;

		FeatherGUI(GLFWwindow* _windowContext, const char* _glsl_version);
		~FeatherGUI();
		
		bool IsOpened();

		void InitGUI();
		void BuildGUI();
		void RenderGUI();

		void SetBackGroundColor(unsigned char _r, unsigned char _g, unsigned char _b);
		RGB GetBackGroundColor();
};

#endif // FEATHERGUI