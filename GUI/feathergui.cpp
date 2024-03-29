#include "feathergui.h"

//TODO
/*
	-> Add all the options to the menu
	-> Add the ability to change the icon size
	-> Add a import export configuration
	-> Add a configuration file
	-> Add a configuration option to enable/disable the default zoom
	-> Make the default zoom configurable
	-> Sort and make the configuration menu more user friendly
	-> Make more tools 
	
	# Search if there is any option to set the images into the executable
*/

//Initialization of static members
std::vector<std::string> FeatherGUI::droppedFiles;

FeatherGUI::FeatherGUI(GLFWwindow* _windowContext, const char* _glsl_version)
{
	this->windowContext = _windowContext;
	glfwGetWindowSize(this->windowContext, &this->windowWidth, &this->windowHeight);

	//Drop callback
	glfwSetDropCallback(this->windowContext, drop_callback);

	// Initialize the Feather GUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	this->io = &ImGui::GetIO();
	(void)io;
	//io->Fonts->AddFontDefault();
	this->CurrentFont = io->Fonts->AddFontFromFileTTF("./resources/fonts/consolas.ttf", 16);
	ImGui::StyleColorsDark();

	this->io->IniFilename = NULL;
	
	//Link the buffer
	this->coutbuff = std::cout.rdbuf();
	std::cout.rdbuf(ss.rdbuf());

	this->MouseImagePositionX = 0;
	this->MouseImagePositionY = 0;
	
	//Windows
	this->isOpen = true;
	this->disableOptionsRounding = true;
	this->CursorEnabled = true;

	this->placementConfig = false;
	this->debugConsole = true;
	this->newImagePopUp = false;
	this->histogramPopUp = false;

	//Error Windows
	errorWindowCode = false;

	//Image Default New Values
	this->newImageWidth = 1000;
	this->newImageHeight = 1000;
	this->newImageTransparency = true;
	
	//Fonts and Text
	this->iconSize = 12.0;
	char szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_FONTS, NULL, 0, szPath)))
	{
		std::string FPath(szPath);
		std::cout << "La ruta de las fuentes es: " << FPath << std::endl;
		this->fontsPath = FPath;
		this->workStation.fontsPath = FPath;
	}

	//Adding Icons
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io->Fonts->AddFontFromFileTTF("./resources/fonts/icon_regular.ttf", this->getIconSize(), &icons_config, icons_ranges);
	//Loading Icons
	if (!loadIcon("./resources/icons/Pencil.png")) {
		std::cout << "Error Loading the Image Pencil.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Brush.png")) {
		std::cout << "Error Loading the Image Brush.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Rubber.png")) {
		std::cout << "Error Loading the Image Rubber.jpg" << std::endl;
	}
	if (!loadIcon("./resources/icons/ColorPicker.png")) {
		std::cout << "Error Loading the Image ColorPicker.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Selection.png")) {
		std::cout << "Error Loading the Image Selection.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Bucket.png")) {
		std::cout << "Error Loading the Image Bucket.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Text.png")) {
		std::cout << "Error Loading the Image Text.png" << std::endl;
	}
	//Load the Black Icons
	if (!loadIcon("./resources/icons/Black_Pencil.png")) {
		std::cout << "Error Loading the Image Black Pencil.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Black_Brush.png")) {
		std::cout << "Error Loading the Image Black Brush.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Black_Rubber.png")) {
		std::cout << "Error Loading the Image Black Rubber.jpg" << std::endl;
	}
	if (!loadIcon("./resources/icons/Black_ColorPicker.png")) {
		std::cout << "Error Loading the Image Black ColorPicker.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Black_Selection.png")) {
		std::cout << "Error Loading the Image Black Selection.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Black_Bucket.png")) {
		std::cout << "Error Loading the Image Black Bucket.png" << std::endl;
	}
	if (!loadIcon("./resources/icons/Black_Text.png")) {
		std::cout << "Error Loading the Image Black Text.png" << std::endl;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_windowContext, true);
	ImGui_ImplOpenGL3_Init(_glsl_version);
	
	this->windowRounding = 0.0F;
	
	//OPTION VARS
	this->Vsync = true;

	//TOOLS VARs
	this->CurrentTool = -1;
	this->ToolPrevisualization = true;
	//Selection
	this->selectionSquareSize = 6;
	this->selectionScaleSquareSize = 6;
	this->topLeft = false;
	this->topRight = false;
	this->bottomLeft = false;
	this->bottomRight = false;
	
	this->BackGroundRGB.r = 64;
	this->BackGroundRGB.g = 64;
	this->BackGroundRGB.b = 64;
	this->BackGroundRGB.delta = 255;
	
	this->whiteIcons = true;

	//Colors of the buttons when a tool is selected and when it is not
	this->colorWindowBar = ImVec4(
		static_cast<float>(this->GetBackGroundColor().r / 255.0F) - 0.5F,
		static_cast<float>(this->GetBackGroundColor().g / 255.0F) - 0.05F,
		static_cast<float>(this->GetBackGroundColor().b / 255.0F) - 0.05F,
		1.0f);
	
	this->colorSelectedWindow = ImVec4(
		static_cast<float>(this->GetBackGroundColor().r / 255.0F) - 0.1F,
		static_cast<float>(this->GetBackGroundColor().g / 255.0F) - 0.1F,
		static_cast<float>(this->GetBackGroundColor().b / 255.0F) - 0.1F,
		1.0f);
	
	this->colorNoSelectedWindow = ImVec4(
		static_cast<float>(this->GetBackGroundColor().r / 255.0F) - 0.90F,
		static_cast<float>(this->GetBackGroundColor().g / 255.0F) - 0.90F,
		static_cast<float>(this->GetBackGroundColor().b / 255.0F) - 0.90F,
		1.0f);
	
	this->colorNoSelectedTool = this->colorSelectedWindow;
	this->colorSelectedTool = ImVec4(
		1.0F - static_cast<float>(this->GetBackGroundColor().r / 255.0F),
		1.0F - static_cast<float>(this->GetBackGroundColor().g / 255.0F),
		1.0F - static_cast<float>(this->GetBackGroundColor().b / 255.0F),
		1.0F);
	this->SliderGrab = ImVec4(0.6F, 0.6F, 0.6F, 1.0F);
	this->SliderGrabActive = ImVec4(0.5F, 0.5F, 0.5F, 1.0F);
	this->ActuatorsBackground = ImVec4(0.35F, 0.35F, 0.35F, 1.0F);

	workStation.init();

	this->MouseOverImageWindow = false;
	this->zoom = 1;
	this->zoomIncrement = 0.1F;
	this->maxZoom = 10.0F;
	this->minZoom = 0.01F;
	this->imageShiftX = 8;
	this->imageShiftY = 8;

	//GUI PLACEMENTS
	this->toolsPanelPixels = 40;
	this->propertiesPanelPixels = 256;
	this->infoPanelPixels = 32;

}

FeatherGUI::~FeatherGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void FeatherGUI::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void FeatherGUI::FloatingText(char* desc) {
	FloatingText((const char*)desc);
}

void FeatherGUI::FloatingText(const char* desc) {
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void FeatherGUI::SetBackGroundColor(unsigned char r, unsigned char g, unsigned char b) {
	this->BackGroundRGB.r = r;
	this->BackGroundRGB.g = g;
	this->BackGroundRGB.b = b;

	//if the colors is white (rgb > 128) set whiteIcons = false
	if (this->BackGroundRGB.r > 128 && this->BackGroundRGB.g > 128 && this->BackGroundRGB.b > 128) {
		this->whiteIcons = false;
	}
	else {
		this->whiteIcons = true;
	}
	
	//Colors of the buttons when a tool is selected and when it is not
	this->colorWindowBar = ImVec4(
		static_cast<float>(this->GetBackGroundColor().r / 255.0F) - 0.05F,
		static_cast<float>(this->GetBackGroundColor().g / 255.0F) - 0.05F,
		static_cast<float>(this->GetBackGroundColor().b / 255.0F) - 0.05F,
		1.0f);
	
	this->colorSelectedWindow = ImVec4(
		static_cast<float>(this->GetBackGroundColor().r / 255.0F) - 0.1F,
		static_cast<float>(this->GetBackGroundColor().g / 255.0F) - 0.1F,
		static_cast<float>(this->GetBackGroundColor().b / 255.0F) - 0.1F,
		1.0f);
	
	this->colorNoSelectedWindow = ImVec4(
		static_cast<float>(this->GetBackGroundColor().r / 255.0F) - 0.90F,
		static_cast<float>(this->GetBackGroundColor().g / 255.0F) - 0.90F,
		static_cast<float>(this->GetBackGroundColor().b / 255.0F) - 0.90F,
		1.0f);
	
	this->colorNoSelectedTool = this->colorSelectedWindow;
	
	this->colorSelectedTool = ImVec4(
		1.0F - static_cast<float>(this->BackGroundRGB.r / 255.0F),
		1.0F - static_cast<float>(this->BackGroundRGB.g / 255.0F),
		1.0F - static_cast<float>(this->BackGroundRGB.b / 255.0F),
		1.0F);
}

RGB FeatherGUI::GetBackGroundColor() {
	return this->BackGroundRGB;
}

void FeatherGUI::InitGUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void FeatherGUI::BuildGUI() {

	glfwGetWindowSize(this->windowContext, &this->windowWidth, &this->windowHeight);
	//Make all menus rounds with window Rounding
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, this->windowRounding);
	//add FrameBorder
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0F);
	
	//Make all menus red
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, this->colorNoSelectedWindow); // Background of the windows
	ImGui::PushStyleColor(ImGuiCol_TabActive, this->colorSelectedWindow); //Tabs in layer and options and selected tool
	ImGui::PushStyleColor(ImGuiCol_Button, this->colorWindowBar); // Window
	//Make Bars and Sliders red
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, this->SliderGrab); // Slider
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, this->SliderGrabActive); // Slider
	ImGui::PushStyleColor(ImGuiCol_FrameBg, this->ActuatorsBackground); // Slider
	
	

	if (this->CursorEnabled) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}
	else {
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	}

	//DROP EVENT
	switch (droppedFiles.size())
	{
	case 0:
		break;
		
	default:
		this->loadDroppedImages();
		break;
	}

	//BUILDGUI
	{
		//--------------------STATIC WINDOWS--------------------
		this->BuildMenu();
		if (this->isOpen) {
			this->BuildTools();
			this->BuildImageDisplayer();
			this->BuildProperties();
			this->BuildLayers();
			this->BuildInfo();

			//--------------------DYNAMIC WINDOWS--------------------
			if (newImagePopUp) {
				this->newImage();
			}
			if (this->placementConfig) {
				this->BuildConfigMenu();
			}
			if (this->debugConsole) {
				this->BuildConsoleDebugMenu();
			}
			if (this->histogramPopUp) {
				this->BuildHistogram();
			}
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	//--------------------INPUT FUNTIONS--------------------
	this->InputFunctions();
}

void FeatherGUI::RenderGUI() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FeatherGUI::SetSync(bool _sync) {
	if(_sync) {
		this->Vsync = true;
		glfwSwapInterval(1);
		std::cout << "Vsync enabled" << std::endl;
	}
	else {
		this->Vsync = false;
		glfwSwapInterval(0);
		std::cout << "Vsync disabled" << std::endl;
	}
}

bool FeatherGUI::GetSync() {
	return this->Vsync;
}

bool FeatherGUI::IsOpened() {
	return this->isOpen;
}

void FeatherGUI::setIconSize(float _size) {
	this->iconSize = _size;
}

float FeatherGUI::getIconSize() {
	return this->iconSize;
}

void FeatherGUI::InputFunctions() {
	//Position of the mouse inside the Image Window compensating the image shift and zoom
	this->MouseImagePositionX = (int)floor(static_cast<float>((this->io->MousePos.x - this->toolsPanelPixels - this->imageShiftX)) / (float)this->zoom);
	this->MouseImagePositionY = (int)floor(static_cast<float>((this->io->MousePos.y - this->MenuSizePixels - this->imageShiftY)) / (float)this->zoom);
	
	//Dragging the Image over the window
	if (ImGui::IsMouseDown(1) && this->MouseOverImageWindow) {
		this->imageShiftX += static_cast<int>(this->io->MouseDelta.x);
		this->imageShiftY += static_cast<int>(this->io->MouseDelta.y);
	}

	//Double click set zoom to 1.0F and the image shift to the center of the Image Window
	if (ImGui::IsMouseDoubleClicked(1)) {
		std::cout << "Current Image: " << this->workStation.getImageStrP()->name << std::endl;
		this->centerImage();
	}

	//UP MOUSE WHEEL
	if (io->MouseWheel > 0 && this->MouseOverImageWindow) {
		//increase lineal zoom by 0.1 if this one is less than 3
		if (this->zoom < this->maxZoom) {
			this->zoom += this->zoomIncrement;
			//Shift the image to zoom in the position of the mouse in the image using MouseImagePositionX and MouseImagePositionY
			this->imageShiftX -= static_cast<int>(this->MouseImagePositionX * this->zoomIncrement);
			this->imageShiftY -= static_cast<int>(this->MouseImagePositionY * this->zoomIncrement);
		}
	}
	//DOWN MOUSE WHEEL
	if (io->MouseWheel < 0 && this->MouseOverImageWindow) {
		//decrease lineal zoom by 0.1 if this one is greater than 0.1
		if (this->zoom > (this->minZoom + this->zoomIncrement)) {
			this->zoom -= this->zoomIncrement;
			//Shift the image to zoom out the position of the mouse in the image using MouseImagePositionX and MouseImagePositionY
			this->imageShiftX += static_cast<int>(this->MouseImagePositionX * this->zoomIncrement);
			this->imageShiftY += static_cast<int>(this->MouseImagePositionY * this->zoomIncrement);
		}
	}
	
	//Selection for the tools
	//If number 1 is pressed select the first tool

	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_1)) {
		this->CurrentTool = TOOL_PENCIL;
	}
	//2
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_2)) {
		this->CurrentTool = TOOL_BRUSH;
	}
	//3
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_3)) {
		this->CurrentTool = TOOL_ERASER;
	}
	//4
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_4)) {
		this->CurrentTool = TOOL_COLORPICKER;
	}
	//5
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_5)) {
		this->CurrentTool = TOOL_SELECTION;
	}
	//6
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_6)) {
		this->CurrentTool = TOOL_BUCKET;
	}
	//7
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_7)) {
		this->CurrentTool = TOOL_TEXT;
	}

	//ESC
	if (io->KeysDown[GLFW_KEY_ESCAPE]) {
		//Close the overlayer windows
		this->placementConfig = false;
		this->debugConsole = false;
		this->newImagePopUp = false;
		//Tool
		//this->CurrentTool = -1;
		//Selection
		this->workStation.clearSelection();
		this->workStation.setSelectionEnable(false);
		this->workStation.setSelectionDone(false);
	}

	//CTRL + C and no mouse button pressed
	if (io->KeysDown[GLFW_KEY_C] && io->KeyCtrl && !ImGui::IsMouseDown(0) && !ImGui::IsMouseDown(1)) {
		if (this->workStation.getSelectionEnabled()) {
			//If selection is active copy the selection to the clipboard
			this->copySelectionToClipboard();
			std::cout << "Selection copied to clipboard" << std::endl;
		}
		else 
		{
			//Copy the image to the clipboard
			this->copyImageToClipboard();
			std::cout << "Image copied to clipboard" << std::endl;
		}
	}

	//CTRL + V
	if (io->KeysDown[GLFW_KEY_V] && io->KeyCtrl) {
		if (this->workStation.getImageStrP()->modified || this->workStation.ImagesSize() == 0) {
			//Paste the image in the clipboard 
			this->loadFromClipBoard();
		}
	}

	//MENU SHORTCUTS
	//CTRL + N
	if (io->KeysDown[GLFW_KEY_N] && io->KeyCtrl) {
		this->newImagePopUp = true;
	}
	//CTRL + O
	if (io->KeysDown[GLFW_KEY_O] && io->KeyCtrl) {
		this->OpenMenuFunction();
	}
	//CTRL + S
	if (io->KeysDown[GLFW_KEY_S] && io->KeyCtrl) {
		this->SaveMenuFunction(0);
	}
	//CTRL + Shift + S
	if (io->KeysDown[GLFW_KEY_S] && io->KeyCtrl && io->KeyShift) {
		this->SaveMenuFunction(1);
	}

	//Key T
	if (io->KeysDown[GLFW_KEY_T]) {
		this->workStation.resizeImage(this->workStation.getImage(0), 1920, 540);
	}

	//Key SUPR
	if (io->KeysDown[GLFW_KEY_DELETE]) {
		if (this->workStation.getSelectionEnabled()) {
			//If selection is active delete the selection
			this->workStation.deleteSelection();
			std::cout << "Selection deleted" << std::endl;
		}
		this->UpdateImage();
		//Remove the selection
		this->workStation.clearSelection();
		this->workStation.setSelectionEnable(false);
		this->workStation.setSelectionDone(false);
	}

	//MOUSE SELECTION
	//If Ctrl is pressed we can move the area
	if (io->KeyCtrl) {
		//If the mouse is over the corners of the selection
		if (this->workStation.getSelectionEnabled() && this->workStation.getSelectionDone() && this->CurrentTool == TOOL_SELECTION) {
			this->selectionUpdate();
		}
		if (topLeft || topRight || bottomLeft || bottomRight || middleLeft || middleRight || middleTop || middleBottom) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
		}
		else {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}
	}
	
	//MOUSE
	// Check if mouse is over the image window
	if (this->MouseOverImageWindow)
	{
		//If the mouse is over the image
		if (this->MouseImagePositionX >= 0 && this->MouseImagePositionX < this->workStation.getImageStrP()->width &&
			this->MouseImagePositionY >= 0 && this->MouseImagePositionY < this->workStation.getImageStrP()->height) {
			//If mouse click or holded
			if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
				if (this->CurrentTool != -1) {
					this->workStation.useTool(this->CurrentTool, this->MouseImagePositionX, this->MouseImagePositionY);
					this->UpdateImage();
				}
			}
		}
	}
	if (ImGui::IsMouseReleased(0)) {
		//Clear the vector of the tool
		workStation.clearMousePoints();

		//Clear Pairs
		workStation.clearMousePairs();

		//Set the firstPoint of Selection to -1
		workStation.clearFirstPointSelection();

		//If the selection is enabled end the selection
		if (this->workStation.getSelectionEnabled()) {
			this->workStation.setSelectionDone(true);
		}

		//Selection Pins resets
		this->topLeft = false;
		this->topRight = false;
		this->bottomLeft = false;
		this->bottomRight = false;
		this->middleLeft = false;
		this->middleRight = false;
		this->middleTop = false;
		this->middleBottom = false;
		this->middle = false;

		//Selection Normalize
		this->workStation.selectionNormalize();
	}
}