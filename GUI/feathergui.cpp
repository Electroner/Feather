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
	-> Search for memory available in the system for creating image
	-> Change and organize the options
	-> Make selection White and black
	
	# Search if there is any option to set the images into the executable
*/

FeatherGUI::FeatherGUI(GLFWwindow* _windowContext, const char* _glsl_version)
{
	this->windowContext = _windowContext;
	glfwGetWindowSize(this->windowContext, &this->windowWidth, &this->windowHeight);

	// Initialize the Feather GUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
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
	this->placementConfig = false;
	this->debugConsole = true;
	this->newImagePopUp = false;

	//Error Windows
	errorWindowCode = false;

	//Image Default New Values
	this->newImageWidth = 1000;
	this->newImageHeight = 1000;
	
	//Fonts and Text
	this->iconSize = 12.0;

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
	if (!loadIcon("./resources/icons/Selection.png")) {
		std::cout << "Error Loading the Image Selection.png" << std::endl;
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
	if (!loadIcon("./resources/icons/Black_Selection.png")) {
		std::cout << "Error Loading the Image Black Selection.png" << std::endl;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_windowContext, true);
	ImGui_ImplOpenGL3_Init(_glsl_version);
	
	this->windowRounding = 0.0F;
	
	//OPTION VARS
	this->Vsync = false;
	
	this->BackGroundRGB.r = 0.25;
	this->BackGroundRGB.g = 0.25;
	this->BackGroundRGB.b = 0.25;
	this->BackGroundRGB.delta = 1.0;
	
	this->whiteIcons = true;

	//Colors of the buttons when a tool is selected and when it is not
	this->colorWindowBar = ImVec4(this->GetBackGroundColor().r - 0.5F, this->GetBackGroundColor().g - 0.05F, this->GetBackGroundColor().b - 0.05F, 1.0f);
	this->colorSelectedWindow = ImVec4(this->GetBackGroundColor().r - 0.1F, this->GetBackGroundColor().g - 0.1F, this->GetBackGroundColor().b - 0.1F, 1.0f);
	this->colorNoSelectedWindow = ImVec4(this->GetBackGroundColor().r - 0.90F, this->GetBackGroundColor().g - 0.90F, this->GetBackGroundColor().b - 0.90F, 1.0f);
	this->colorNoSelectedTool = this->colorSelectedWindow;
	this->colorSelectedTool = ImVec4(1.0F - this->BackGroundRGB.r, 1.0F - this->BackGroundRGB.g, 1.0F - this->BackGroundRGB.b, 1.0F);

	workStation.init();

	this->MouseOverImageWindow = false;
	this->zoom = 1;
	this->zoomIncrement = 0.1F;
	this->maxZoom = 10.0F;
	this->minZoom = 0.01F;
	this->imageShiftX = 8;
	this->imageShiftY = 8;

	//GUI PLACEMENTS
	this->toolsPanelPixels = 32;
	this->propertiesPanelPixels = 256;
	this->infoPanelPixels = 32;

	//TOOLS VARs
	this->CurrentTool = -1;
}

FeatherGUI::~FeatherGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
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

void FeatherGUI::SetBackGroundColor(float r, float g, float b) {
	this->BackGroundRGB.r = r;
	this->BackGroundRGB.g = g;
	this->BackGroundRGB.b = b;

	//if the colors is white (rgb > 128) set whiteIcons = false
	if (this->BackGroundRGB.r > 0.5 && this->BackGroundRGB.g > 0.5 && this->BackGroundRGB.b > 0.5) {
		this->whiteIcons = false;
	}
	else {
		this->whiteIcons = true;
	}
	
	//Colors of the buttons when a tool is selected and when it is not
	this->colorWindowBar = ImVec4(this->GetBackGroundColor().r - 0.05F, this->GetBackGroundColor().g - 0.05F, this->GetBackGroundColor().b - 0.05F, 1.0f);
	this->colorSelectedWindow = ImVec4(this->GetBackGroundColor().r - 0.1F, this->GetBackGroundColor().g - 0.1F, this->GetBackGroundColor().b - 0.1F, 1.0f);
	this->colorNoSelectedWindow = ImVec4(this->GetBackGroundColor().r - 0.90F, this->GetBackGroundColor().g - 0.90F, this->GetBackGroundColor().b - 0.90F, 1.0f);
	this->colorNoSelectedTool = this->colorSelectedWindow;
	this->colorSelectedTool = ImVec4(1.0F - this->BackGroundRGB.r, 1.0F - this->BackGroundRGB.g, 1.0F - this->BackGroundRGB.b, 1.0F);
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
	//Make all menus red
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, this->colorNoSelectedWindow); // Background of the windows
	ImGui::PushStyleColor(ImGuiCol_TabActive, this->colorSelectedWindow); //Tabs in layer and options and selected tool
	ImGui::PushStyleColor(ImGuiCol_Button, this->colorWindowBar); // Window
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
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
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
	//Dragging the Image over the window
	if (ImGui::IsMouseDown(1)) {
		this->imageShiftX += static_cast<int>(this->io->MouseDelta.x);
		this->imageShiftY += static_cast<int>(this->io->MouseDelta.y);
	}

	//Double click set zoom to 1.0F and the image shift to the center of the Image Window
	if (ImGui::IsMouseDoubleClicked(1)) {
		std::cout << "Current Image: " << this->workStation.getImageStrP()->name << std::endl;
		this->centerImage();
	}

	//UP MOUSE WHEEL
	if (io->MouseWheel > 0) {
		//increase lineal zoom by 0.1 if this one is less than 3
		if (this->zoom < this->maxZoom) {
			this->zoom += this->zoomIncrement;
			//Shift the image to zoom in the position of the mouse in the image using MouseImagePositionX and MouseImagePositionY
			this->imageShiftX -= static_cast<int>(this->MouseImagePositionX * this->zoomIncrement);
			this->imageShiftY -= static_cast<int>(this->MouseImagePositionY * this->zoomIncrement);
		}
	}
	//DOWN MOUSE WHEEL
	if (io->MouseWheel < 0) {
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
	if (ImGui::IsKeyPressed(49)) {
		this->CurrentTool = 0;
	}
	//2
	if (ImGui::IsKeyPressed(50)) {
		this->CurrentTool = 1;
	}
	//3
	if (ImGui::IsKeyPressed(51)) {
		this->CurrentTool = 2;
	}
	//4
	if (ImGui::IsKeyPressed(52)) {
		this->CurrentTool = 3;
	}

	//ESC
	if (io->KeysDown[GLFW_KEY_ESCAPE]) {
		//Close the overlayer windows
		this->placementConfig = false;
		this->debugConsole = false;
		this->newImagePopUp = false;
		//Tool
		this->CurrentTool = -1;
		//Selection
		this->workStation.clearSelection();
		this->workStation.setSelectionEnable(false);
	}

	//CTRL + C
	if (io->KeysDown[GLFW_KEY_C] && io->KeyCtrl) {
		if (this->workStation.getSelectionEnabled()) {
			//If selection is active copy the selection to the clipboard
			this->copySelectionToClipboard();
		}
		else 
		{
			//Copy the image to the clipboard
			this->copyImageToClipboard();
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
		this->workStation.resizeImage(this->workStation.getImageStrP(), 1920, 540);
		this->workStation.reCopyImage(this->workStation.getImageStrP());
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
					workStation.useTool(this->CurrentTool, this->MouseImagePositionX, this->MouseImagePositionY);
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
	}
	
	//Position of the mouse inside the Image Window compensating the image shift and zoom
	this->MouseImagePositionX = (int)floor(static_cast<float>((this->io->MousePos.x - this->toolsPanelPixels - this->imageShiftX)) / (float)this->zoom);
	this->MouseImagePositionY = (int)floor(static_cast<float>((this->io->MousePos.y - this->MenuSizePixels - this->imageShiftY)) / (float)this->zoom);
}