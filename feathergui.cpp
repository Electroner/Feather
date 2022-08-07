#include "feathergui.h"
#include "pathwork.h"
#include "IconsFontAwesome.h"

void FeatherGUI::BuildGUI() {
	
	//LOADING IMAGES
	if (Images.empty()) {
		if (!loadImage("./resoruces/exampleimages/manzana.png")) {
			std::cout << "Error Loading the Image manzana.png" << std::endl;
		}
		if (!loadImage("./resoruces/exampleimages/naranja.png")) {
			std::cout << "Error Loading the Image naranja.png" << std::endl;
		}
		if (!loadImage("./resoruces/exampleimages/nebula.jpg")) {
			std::cout << "Error Loading the Image nebula.jpg" << std::endl;
		}
		CurrentImage = Images.front();
		std::cout << "Current Image Properties: " << std::endl;
		std::cout << "Identifier: " << CurrentImage.texture << std::endl;
		std::cout << "Size = " << CurrentImage.width << "x" << CurrentImage.height << std::endl;
		std::cout << "Channels: " << CurrentImage.channels << std::endl;
		this->centerImage();
	}
	
	glfwGetWindowSize(this->windowContext, &this->windowWidth, &this->windowHeight);
	//Make all menus rounds with window Rounding
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, this->windowRounding);
	//BUILDGUI
	{
		//--------------------STATIC WINDOWS--------------------
		this->BuildMenu();
		this->BuildTools();
		this->BuildImageDisplayer();
		this->BuildProperties();
		this->BuildLayers();
		this->BuildInfo();
		
		//--------------------DYNAMIC WINDOWS--------------------
		if (this->placementConfig) {
			this->BuildConfigMenu();
		}
		if (this->debugConsole) {
			this->BuildConsoleDebugMenu();
		}
	}
	ImGui::PopStyleVar();
	
	//--------------------INPUT FUNTIONS--------------------
	this->InputFunctions();
}

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
	this->CurrentFont = io->Fonts->AddFontFromFileTTF("./resoruces/consolas.ttf", 16);
	ImGui::StyleColorsDark();
	
	//Link the buffer
	this->coutbuff = std::cout.rdbuf();
	std::cout.rdbuf(ss.rdbuf());

	this->MouseImagePositionX = 0;
	this->MouseImagePositionY = 0;


	//Windows
	this->isOpen = true;
	this->debugConsole = true;
	this->placementConfig = false;
	this->disableOptionsRounding = true;

	//Fonts and Text
	this->iconSize = 12.0;

	//Adding Icons
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io->Fonts->AddFontFromFileTTF("./resoruces/icon_regular.ttf", this->getIconSize(), &icons_config, icons_ranges);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_windowContext, true);
	ImGui_ImplOpenGL3_Init(_glsl_version);
	
	this->windowRounding = 10.0F;
	
	//OPTION VARS
	this->Vsync = true;
	
	this->BackGroundRGB.r = 0.25;
	this->BackGroundRGB.g = 0.25;
	this->BackGroundRGB.b = 0.25;
	this->BackGroundRGB.delta = 1.0;

	//set to false every loaded image in the vector
	for (int i = 0; i < Images.size(); i++) {
		Images[i].loaded = false;
	}

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

void FeatherGUI::SetBackGroundColor(float r, float g, float b) {
	this->BackGroundRGB.r = r;
	this->BackGroundRGB.g = g;
	this->BackGroundRGB.b = b;
}

RGB FeatherGUI::GetBackGroundColor() {
	return this->BackGroundRGB;
}

void FeatherGUI::InitGUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void FeatherGUI::RenderGUI() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool FeatherGUI::loadImage(std::string _path) {
	//Load texture from file
	ImageStr unloadedImage;
	unloadedImage.imagePath = _path;
	std::cout << "Loading image: " << _path << std::endl;
	unloadedImage.data = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &unloadedImage.channels, 3);
	if (!unloadedImage.data) {
		fprintf(stderr, "Cannot load image '%s'\n", _path.c_str());
		unloadedImage.loaded = true;
		return false;
	}

	//Set the name of the image from the path
	unloadedImage.name = _path.substr(_path.find_last_of('/') + 1);
	unloadedImage.extension = _path.substr(_path.find_last_of('.') + 1);
	std::cout << "Image extension: " << unloadedImage.extension << std::endl;
	//Erase the extension of the name
	unloadedImage.name = unloadedImage.name.substr(0, unloadedImage.name.find_last_of('.'));
	std::cout << "Image name: " << unloadedImage.name << std::endl << std::endl;

	// Create a OpenGL texture identifier and binding
	glGenTextures(1, &unloadedImage.texture);
	glBindTexture(GL_TEXTURE_2D, unloadedImage.texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	// Upload pixels into texture
	#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	#endif
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, unloadedImage.width, unloadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	stbi_image_free(unloadedImage.data);
	
	unloadedImage.loaded = true;

	//Add image to Images vector
	Images.push_back(unloadedImage);

	return true;
}

void FeatherGUI::SetSync(bool _sync) {
	if(_sync) {
		this->Vsync = true;
		glfwSwapInterval(1);
	}
	else {
		this->Vsync = false;
		glfwSwapInterval(0);
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

void FeatherGUI::calculateZoom() {
	//Whit the size of the image and the size of the screen, calculate the zoom to fit the image in the screen letting 32 pixels of space on the closest side
	//If the image is taller than widder then the image will be zoomed in vertically and horizontally
	//If the image is wider than taller then the image will be zoomed in horizontally and vertically
	
	//Window Sizes
	int size_x = (1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x;
	int size_y = io->DisplaySize.y - this->MenuSizePixels - this->infoPanelPixels;

	//Image Sizes
	int image_x = CurrentImage.width;
	int image_y = CurrentImage.height;

	//Calculate the zoom
	if (image_x / size_x > image_y / size_y) {
		this->zoom = (float)size_x / ((float)image_x + 32.0F);
	}
	else {
		this->zoom = (float)size_y / ((float)image_y + 32.0F);
	}
	
	//TODO Improve this
	
	//Set the maximun to fit a pixel in a 100 screen pixels
	this->maxZoom = ((float)image_x) / 100.0F;
	
	//Set the minimun to fit 100 pixels in a pixel
	this->minZoom = 100.0F / ((float)image_x);

	//Set the increment in the 1% of the diference between min and max
	this->zoomIncrement = (this->maxZoom - this->minZoom) / 100.0F;
}

void FeatherGUI::centerImage() {
	calculateZoom();
	//Center X is (1-(x+z))/2+x, where x is toolsPanelPixels and z is propertiesPanelPixels, substract the size of the image
	this->imageShiftX = static_cast<int>((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x / 2.0F - (CurrentImage.width * this->zoom / 2.0F));
	//Center Y is the size of the screen minus the size of the image divided by 2
	this->imageShiftY = static_cast<int>((io->DisplaySize.y - this->infoPanelPixels - this->MenuSizePixels) / 2.0F - (CurrentImage.height * this->zoom / 2.0F));

}

void FeatherGUI::BuildMenu() {
	//Create a main menu
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu(ICON_FA_FILE_ALT " File")) {
			if (ImGui::MenuItem(ICON_FA_FILE_IMAGE " New", "|Ctrl+N")) {

			}
			if (ImGui::MenuItem(ICON_FA_FILE " Open", "|Ctrl+O")) {
				std::string filename;
				filename = browse(0);
				//load the image
				if (!loadImage(filename)) {
					ImGui::Text("Error loading image");
				}
				this->CurrentImage = this->Images.back();
			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save", "|Ctrl+S")) {

			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save As", "|Ctrl+Shift+S")) {

			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Close", "|Alt+F4")) {
				this->isOpen = false;
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();
		//Edit
		if (ImGui::BeginMenu(ICON_FA_EDIT " Edit")) {
			ImGui::EndMenu();
		}
		//Add a separator
		ImGui::Separator();
		//Layer
		if (ImGui::BeginMenu(ICON_FA_IMAGES " Layer")) {
			ImGui::EndMenu();
		}
		ImGui::Separator();
		//Filter
		if (ImGui::BeginMenu(ICON_FA_OBJECT_UNGROUP " Filter")) {
			ImGui::EndMenu();
		}
		ImGui::Separator();
		//Windows
		if (ImGui::BeginMenu(ICON_FA_WINDOW_MAXIMIZE " Windows")) {
			ImGui::EndMenu();
		}
		ImGui::Separator();
		//Settings
		if (ImGui::BeginMenu(ICON_FA_SUN " Settings")) {
			if (ImGui::MenuItem(" GUI Placements", "|Ctrl+Alt+D")) {
				std::cout << "Placement GUI opened" << std::endl;
				this->placementConfig = true;
			}
			if (ImGui::MenuItem(" GUI Colors", "|Ctrl+Alt+G")) {

			}
			if (ImGui::MenuItem(" GUI Fonts", "|Ctrl+Alt+F")) {

			}
			if (ImGui::MenuItem(" Open Debug Log", "|Ctrl+Alt+C")) {
				this->debugConsole = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void FeatherGUI::BuildTools() {
	//TOOLS WINDOW
	//Create a window on the left maximized that occupies toolsPanelPixels of the space.
	ImGui::SetNextWindowPos(ImVec2(0, (float)this->MenuSizePixels), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float)(static_cast<float>(this->toolsPanelPixels) / static_cast<float>(this->windowWidth)) * io->DisplaySize.x, io->DisplaySize.y - this->MenuSizePixels - this->infoPanelPixels - 1), ImGuiCond_Always);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(this->GetBackGroundColor().r - 0.05F, this->GetBackGroundColor().g - 0.05F, this->GetBackGroundColor().b - 0.05F, 1.0f));
	ImGui::Begin("Tools Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	//TODO
	//TODOEND
	ImGui::End();
	ImGui::PopStyleColor();

	//MAIN SEPARATOR
	ImGui::Separator();
}

void FeatherGUI::BuildImageDisplayer() {
	//IMAGE WINDOW
	ImGui::SetNextWindowPos(ImVec2((float)(static_cast<float>(this->toolsPanelPixels) / static_cast<float>(this->windowWidth)) * (float)io->DisplaySize.x, (float)this->MenuSizePixels), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x, io->DisplaySize.y - this->MenuSizePixels - this->infoPanelPixels), ImGuiCond_Always);
	ImGui::Begin("Image Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);
	//TODO
	//Move the image
	ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
	//Draw Image
	if (!this->Images.empty()) {
		if (CurrentImage.data != NULL) {
			ImGui::Image((void*)(intptr_t)CurrentImage.texture, ImVec2(CurrentImage.width * this->zoom, CurrentImage.height * this->zoom));
		}
	}
	//TODOEND
	ImGui::End();

	//MAIN SEPARATOR
	ImGui::Separator();
}

void FeatherGUI::BuildProperties() {
	//PROPERTIES WINDOW
	float temp_percentage = ((float)(static_cast<float>(this->propertiesPanelPixels) / static_cast<float>(this->windowWidth)));
	ImGui::SetNextWindowPos(ImVec2((1.0F - temp_percentage) * this->io->DisplaySize.x, (float)this->MenuSizePixels), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(temp_percentage * this->io->DisplaySize.x, (this->io->DisplaySize.y - this->MenuSizePixels) / 2), ImGuiCond_Always);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(this->GetBackGroundColor().r - 0.05F, this->GetBackGroundColor().g - 0.05F, this->GetBackGroundColor().b - 0.05F, 1.0f));
	ImGui::Begin("Properties Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	//TODO
	//TabItem
	if (ImGui::BeginTabBar("Properties", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Images")) {
			ImGui::Text("Data of the Image");
			ImGui::Separator();
			ImGui::Text("Identifier: %d", CurrentImage.texture);
			ImGui::Text("Size = %d x %d", CurrentImage.width, CurrentImage.height);
			ImGui::Text("Channels: %d", CurrentImage.channels);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
	
	//MAIN SEPARATOR
	ImGui::Separator();
}

void FeatherGUI::BuildLayers() {
	//LAYER WINDOW
	float temp_percentage = ((float)(static_cast<float>(this->propertiesPanelPixels) / static_cast<float>(this->windowWidth)));
	ImGui::SetNextWindowPos(ImVec2((1.0F - temp_percentage) * this->io->DisplaySize.x, (this->io->DisplaySize.y + this->MenuSizePixels) / 2), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(temp_percentage * this->io->DisplaySize.x, (this->io->DisplaySize.y - this->MenuSizePixels) / 2 - this->infoPanelPixels), ImGuiCond_Always);
	ImGui::Separator();
	//TODO
	ImGui::Begin("Layers Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	if (ImGui::BeginTabBar("Layers", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Layers")) {
			for (int i = 0; i < this->Images.size(); i++) {
				ImGui::Image((void*)(intptr_t)this->Images[i].texture, ImVec2(100 * temp_percentage, 100 * temp_percentage));
				ImGui::SameLine();
				ImGui::Selectable(this->Images[i].name.c_str());
				if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
				{
					int n_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
					if (n_next >= 0 && n_next < this->Images.size())
					{
						std::swap(this->Images[i], this->Images[n_next]);
						ImGui::ResetMouseDragDelta();
						CurrentImage = Images.front();
						centerImage();
					}
				}
				ImGui::Separator();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	//TODOEND
	ImGui::End();
	ImGui::PopStyleColor();

	//MAIN SEPARATOR
	ImGui::Separator();
}

void FeatherGUI::BuildInfo() {
	//INFO WINDOW
	ImGui::SetNextWindowPos(ImVec2(0, this->io->DisplaySize.y - this->infoPanelPixels), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(this->io->DisplaySize.x, (float)this->infoPanelPixels), ImGuiCond_Always);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(this->GetBackGroundColor().r - 0.05F, this->GetBackGroundColor().g - 0.05F, this->GetBackGroundColor().b - 0.05F, 1.0f));
	//Adjust the font size to fit the window
	ImGui::Begin("Info Bar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	//TODO
	{
		//Check first if the mouse is over the Image Window
		if (this->MouseImagePositionX >= 0 && this->MouseImagePositionX <= this->CurrentImage.width &&
			this->MouseImagePositionY >= 0 && this->MouseImagePositionY <= this->CurrentImage.height) {
			ImGui::Text("Current Pixel: (%d, %d)", this->MouseImagePositionX, this->MouseImagePositionY);
		}
		else
		{
			ImGui::Text("Current Pixel: (-,-)");
		}
		//Print the zoom on the same line
		ImGui::SameLine();
		ImGui::Text(" |  Zoom: %.2f", this->zoom);
	}
	//TODOEND
	ImGui::End();
	ImGui::PopStyleColor();
}

void FeatherGUI::BuildConfigMenu() {
	//PLACEMENT CONFIG WINDOW
	ImGui::Begin("Placement Config", &this->placementConfig);
	//Create a slider for the toolsPanelPercentage
	ImGui::SliderInt("Tools Panel Pixels", &this->toolsPanelPixels, 32, 128);
	//Create a slider for the propertiesPanelPercentage
	ImGui::SliderInt("Properties Panel Pixels", &this->propertiesPanelPixels, 128, 512);

	float color[4] = { GetBackGroundColor().r,GetBackGroundColor().g, GetBackGroundColor().b, 1.0F };
	//Change background color
	ImGui::ColorEdit3("Background Color", color);
	SetBackGroundColor(color[0], color[1], color[2]);

	//Separator
	ImGui::Separator();
	ImGui::Text("Rounding Options");
	ImGui::SliderFloat("windowRounding", &this->windowRounding, 0.0F, 10.0F);
	ImGui::SameLine();
	HelpMarker("Disable rounding by setting this to 0.0");
	//Disable options rounding
	ImGui::Checkbox("Options Rounding", &this->disableOptionsRounding);
	if (this->disableOptionsRounding) {
		ImGui::GetStyle().FrameRounding = 0.0F;
	}
	else
	{
		ImGui::GetStyle().FrameRounding = 5.0F;
	}
	//Enable/disable sync
	bool tempsync = this->Vsync;
	ImGui::Checkbox("Vertical Sync", &tempsync);
	if (tempsync != this->Vsync) {
		this->SetSync(tempsync);
	}
	ImGui::End();
}

void FeatherGUI::BuildConsoleDebugMenu() {
	//DEBUG CONSOLE WINDOW
	ImGui::Begin("Debug Console", &this->debugConsole, ImGuiWindowFlags_NoCollapse);
	//FPS
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//Print the mouse
	ImGui::Text("Mouse Position: (%d, %d)", this->MouseImagePositionX, this->MouseImagePositionY);
	//Slider int
	ImGui::Text("Movement X: %d", this->imageShiftX);
	ImGui::Text("Movement Y: %d", this->imageShiftY);
	//Print the zoom
	ImGui::Text("Zoom: %.2f", this->zoom);
	//Print the maximun zoom
	ImGui::Text("Max Zoom: %.2f", this->maxZoom);
	//Print the minimun zoom
	ImGui::Text("Min Zoom: %.2f", this->minZoom);
	
	
	ImGui::Separator();

	//if (ss.str().c_str() != NULL) {
	//	//Get time and add a time mark using localtime_s
	//	time_t rawtime;
	//	struct tm now;
	//	char timeMark[80];
	//	time(&rawtime);
	//	localtime_s(&now, &rawtime);
	//	strftime(timeMark, 80, "%H:%M:%S", &now);			
	//	//Print the time mark
	//}

	ImGui::Text("%s", ss.str().c_str());
	ImGui::End();
}

void FeatherGUI::InputFunctions() {
	//Dragging the Image over the window
	if (ImGui::IsMouseDown(1)) {
		this->imageShiftX += static_cast<int>(this->io->MouseDelta.x);
		this->imageShiftY += static_cast<int>(this->io->MouseDelta.y);
	}

	//Double click set zoom to 1.0F and the image shift to the center of the Image Window
	if (ImGui::IsMouseDoubleClicked(1)) {
		this->centerImage();
	}

	//UP
	if (io->MouseWheel > 0) {
		//increase lineal zoom by 0.1 if this one is less than 3
		if (this->zoom < this->maxZoom) {
			this->zoom += this->zoomIncrement;
		}
	}
	//DOWN
	if (io->MouseWheel < 0) {
		//decrease lineal zoom by 0.1 if this one is greater than 0.1
		if (this->zoom > this->minZoom) {
			this->zoom -= this->zoomIncrement;
		}
	}

	//MOUSE

	//Position of the mouse inside the Image Window
	//this->MouseImagePositionX = this->io->MousePos.x - this->toolsPanelPixels - this->SeparatorSizePixels;
	//this->MouseImagePositionY = this->io->MousePos.y - this->MenuSizePixels - this->SeparatorSizePixels;

	//Same but compensating the image shift and zoom
	this->MouseImagePositionX = (int)ceil(static_cast<float>((this->io->MousePos.x - this->toolsPanelPixels - this->imageShiftX)) / (float)this->zoom);
	this->MouseImagePositionY = (int)ceil(static_cast<float>((this->io->MousePos.y - this->MenuSizePixels - this->imageShiftY)) / (float)this->zoom);
}
