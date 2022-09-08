#include "feathergui.h"
#include "pathwork.h"
#include "IconsFontAwesome.h"

//TODO
/*
	-> Add all the options to the menu
	-> Add the ability to change the icon size
	-> Add a import export configuration
	-> Add a configuration file
	-> Fix the zoom
	-> Add a configuration option to enable/disable the default zoom
	-> Make the default zoom configurable
	-> Sort and make the configuration menu more user friendly
	-> Create a move all the functionality of the image to a new ImageWork.h / ImageWork.cpp ~~
	
	# Search if there is any option to set the images into the executable
*/

void FeatherGUI::BuildGUI() {
	
	//LOADING IMAGES
	if (this->Images->empty()) {
		if (!loadImage("./resoruces/exampleimages/test.png")) {
			std::cout << "Error Loading the Image test.png" << std::endl;
		}

		*this->CurrentImage = this->Images->front();
		this->centerImage();
	}
	
	glfwGetWindowSize(this->windowContext, &this->windowWidth, &this->windowHeight);
	//Make all menus rounds with window Rounding
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, this->windowRounding);
	//Make all menus red
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, this->colorNoSelectedWindow); // Background of the windows
	ImGui::PushStyleColor(ImGuiCol_TabActive, this->colorSelectedWindow); //Tabs in layer and options and selected tool
	ImGui::PushStyleColor(ImGuiCol_Button, this->colorWindowBar); // Ventana
	//BUILDGUI
	{
		//--------------------STATIC WINDOWS--------------------
		this->BuildMenu();
		if(this->isOpen){
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
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
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
	this->CurrentFont = io->Fonts->AddFontFromFileTTF("./resoruces/fonts/consolas.ttf", 16);
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
	io->Fonts->AddFontFromFileTTF("./resoruces/fonts/icon_regular.ttf", this->getIconSize(), &icons_config, icons_ranges);
	//Loading Icons
	if (!loadIcon("./resoruces/icons/Pencil.png")) {
		std::cout << "Error Loading the Image Pencil.png" << std::endl;
	}
	if (!loadIcon("./resoruces/icons/Brush.png")) {
		std::cout << "Error Loading the Image Brush.png" << std::endl;
	}
	if (!loadIcon("./resoruces/icons/Rubber.png")) {
		std::cout << "Error Loading the Image Rubber.jpg" << std::endl;
	}
	//Load the Black Icons
	if (!loadIcon("./resoruces/icons/Black_Pencil.png")) {
		std::cout << "Error Loading the Image Black Pencil.png" << std::endl;
	}
	if (!loadIcon("./resoruces/icons/Black_Brush.png")) {
		std::cout << "Error Loading the Image Black Brush.png" << std::endl;
	}
	if (!loadIcon("./resoruces/icons/Black_Rubber.png")) {
		std::cout << "Error Loading the Image Black Rubber.jpg" << std::endl;
	}

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
	
	this->whiteIcons = true;

	//Colors of the buttons when a tool is selected and when it is not
	this->colorWindowBar = ImVec4(this->GetBackGroundColor().r - 0.5F, this->GetBackGroundColor().g - 0.05F, this->GetBackGroundColor().b - 0.05F, 1.0f);
	this->colorSelectedWindow = ImVec4(this->GetBackGroundColor().r - 0.1F, this->GetBackGroundColor().g - 0.1F, this->GetBackGroundColor().b - 0.1F, 1.0f);
	this->colorNoSelectedWindow = ImVec4(this->GetBackGroundColor().r - 0.90F, this->GetBackGroundColor().g - 0.90F, this->GetBackGroundColor().b - 0.90F, 1.0f);
	this->colorNoSelectedTool = this->colorSelectedWindow;
	this->colorSelectedTool = ImVec4(1.0F - this->BackGroundRGB.r, 1.0F - this->BackGroundRGB.g, 1.0F - this->BackGroundRGB.b, 1.0F);

	workStation.init(&Images,&CurrentImage);
	
	//set to false every loaded image in the vector
	for (int i = 0; i < this->Images->size(); i++) {
		Images->at(i).loaded = false;
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

void FeatherGUI::RenderGUI() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool FeatherGUI::loadImage(std::string _path) {
	//Load texture from file
	ImageStr unloadedImage;
	unloadedImage.imagePath = _path;
	std::cout << "Loading image: " << _path << std::endl;
	//Get the numbers of channels in the image
	int channels;
	unsigned char* image = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &channels, 0);
	if (!image) {
		std::cout << "Error Loading the Image: " << _path << std::endl;
		return false;
	}
	unloadedImage.data = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &unloadedImage.channels, channels);
	if (!unloadedImage.data) {
		fprintf(stderr, "Cannot load image '%s'\n", _path.c_str());
		unloadedImage.loaded = true;
		return false;
	}

	//free image
	stbi_image_free(image);

	//Set the name of the image from the path
	unloadedImage.name = _path.substr(_path.find_last_of('/') + 1);
	unloadedImage.extension = _path.substr(_path.find_last_of('.') + 1);
	std::cout << "Image extension: " << unloadedImage.extension << std::endl;
	//Erase the extension of the name
	unloadedImage.name = unloadedImage.name.substr(0, unloadedImage.name.find_last_of('.'));
	std::cout << "Image name: " << unloadedImage.name << std::endl << std::endl;

	//Enable transparency
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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
	if(channels == 4) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, unloadedImage.width, unloadedImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);
	} 
	else 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, unloadedImage.width, unloadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	stbi_image_free(unloadedImage.data);
	
	//Reserve memory for the image
	unloadedImage.data = new GLubyte[unloadedImage.width * unloadedImage.height * unloadedImage.channels];
	
	if (channels == 4) 
	{
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	else
	{
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	
	unloadedImage.loaded = true;

	//Add image to Images vector
	this->Images->push_back(unloadedImage);

	return true;
}

bool FeatherGUI::loadIcon(std::string _path) {
	//Load texture from file
	ImageStr unloadedImage;
	unloadedImage.imagePath = _path;
	std::cout << "Loading icon: " << _path << std::endl;
	int channels;
	unsigned char* image = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &channels, 0);
	if (!image) {
		std::cout << "Error Loading the icon: " << _path << std::endl;
		return false;
	}
	unloadedImage.data = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &unloadedImage.channels, channels);
	if (!unloadedImage.data) {
		fprintf(stderr, "Cannot load icon '%s'\n", _path.c_str());
		unloadedImage.loaded = true;
		return false;
	}

	//free image
	stbi_image_free(image);

	//Set the name of the image from the path
	unloadedImage.name = _path.substr(_path.find_last_of('/') + 1);
	unloadedImage.extension = _path.substr(_path.find_last_of('.') + 1);
	std::cout << "Icon extension: " << unloadedImage.extension << std::endl;
	//Erase the extension of the name
	unloadedImage.name = unloadedImage.name.substr(0, unloadedImage.name.find_last_of('.'));
	std::cout << "Icon name: " << unloadedImage.name << std::endl << std::endl;

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
	if (channels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, unloadedImage.width, unloadedImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, unloadedImage.width, unloadedImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, unloadedImage.data);
	}
	stbi_image_free(unloadedImage.data);

	unloadedImage.loaded = true;

	//Add image to Images vector
	toolsIcons.push_back(unloadedImage);

	return true;
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

void FeatherGUI::calculateZoom() {
	//Whit the size of the image and the size of the screen, calculate the zoom to fit the image in the screen letting 32 pixels of space on the closest side
	//If the image is taller than widder then the image will be zoomed in vertically and horizontally
	//If the image is wider than taller then the image will be zoomed in horizontally and vertically
	
	//Window Sizes
	int size_x = (1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x;
	int size_y = io->DisplaySize.y - this->MenuSizePixels - this->infoPanelPixels;

	//Image Sizes
	int image_x = this->CurrentImage->width;
	int image_y = this->CurrentImage->height;

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

	std::cout << "Adjusted Zoom to: " << this->zoom << std::endl;
	std::cout << "Adjusted Max Zoom: " << this->maxZoom << std::endl;
	std::cout << "Adjusted Min Zoom: " << this->minZoom << std::endl;
	std::cout << "Adjusted Zoom Increment: " << this->zoomIncrement << std::endl;
}

void FeatherGUI::centerImage() {
	calculateZoom();
	//Center X is (1-(x+z))/2+x, where x is toolsPanelPixels and z is propertiesPanelPixels, substract the size of the image
	this->imageShiftX = static_cast<int>((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x / 2.0F - (this->CurrentImage->width * this->zoom / 2.0F));
	//Center Y is the size of the screen minus the size of the image divided by 2
	this->imageShiftY = static_cast<int>((io->DisplaySize.y - this->infoPanelPixels - this->MenuSizePixels) / 2.0F - (this->CurrentImage->height * this->zoom / 2.0F));

	std::cout << "Adjusted Image Shift X: " << this->imageShiftX << std::endl;
	std::cout << "Adjusted Image Shift Y: " << this->imageShiftY << std::endl;
}

void FeatherGUI::UpdateImage() {
	//Select the current texture
	glBindTexture(GL_TEXTURE_2D,this->CurrentImage->texture);
	//Render back the texture of opengl
	if (this->CurrentImage->channels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->CurrentImage->width, this->CurrentImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->CurrentImage->data);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->CurrentImage->width, this->CurrentImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->CurrentImage->data);
	}
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
				//If the filename doesnt have ":" error			
				if (filename.find(':') == std::string::npos) {
					std::cout << "No file selected" << std::endl;
				}
				else
				{
					//load the image
					if (!loadImage(filename)) 
					{
						std::cout << "Error loading image" << std::endl;
					}
					else 
					{
						*this->CurrentImage = this->Images->front();
					}
				}
			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save", "|Ctrl+S")) {

			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save As", "|Ctrl+Shift+S")) {

			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Close", "|Alt+F4")) {
				//Free every image
				for (int i = 0; i < this->Images->size(); i++) {
					//Free the image
					glDeleteTextures(1, &this->Images->at(i).texture);
					//delete Image data
					delete this->Images->at(i).data;
				}
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
	ImGui::PushStyleColor(ImGuiCol_WindowBg, this->colorSelectedWindow);
	ImGui::Begin("Tools Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	//TODO
	ImVec4 colorActualButton;
	//For every icon in the folder
	int temp_i;
	for (int i = 0; i < this->toolsIcons.size()/2; i++) {
		temp_i = i;
		if (this->whiteIcons) {
			temp_i = i;
		}
		else 
		{
			temp_i = i + this->toolsIcons.size() / 2;
		}
		//Select the color for selected icon
		if (this->CurrentTool == i) 
		{
			colorActualButton = this->colorSelectedTool;
			if (this->whiteIcons) 
			{
				colorActualButton = this->colorSelectedTool;
				temp_i = i + this->toolsIcons.size() / 2;
			}
			else 
			{
				colorActualButton = this->colorSelectedTool;
				temp_i = i;
			}
		}
		else
		{
			colorActualButton = this->colorNoSelectedTool;
		}
		//Create a iconbutton for each tool using the images of the icons vector with transparent background
		if (ImGui::ImageButton((void*)(intptr_t)this->toolsIcons[temp_i].texture, ImVec2(this->toolsPanelPixels / 2, this->toolsPanelPixels / 2), ImVec2(0, 0), ImVec2(1, 1), 0, colorActualButton)) {
			//create a floatingtext with index
			if(this->CurrentTool == i)
			{
				std::cout << "Tool " << i << " deselected" << std::endl;
				this->CurrentTool = -1;
			}
			else
			{
				std::cout << "Tool " << i << " selected" << std::endl;
				this->CurrentTool = i;
			}
		}
	}
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

	//Move the image
	ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
	//Draw Image
	if (!this->Images->empty()) {
		if (this->CurrentImage->data != NULL) {
			ImGui::Image((void*)(intptr_t)this->CurrentImage->texture, ImVec2(this->CurrentImage->width * this->zoom, this->CurrentImage->height * this->zoom));
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
	ImGui::PushStyleColor(ImGuiCol_WindowBg, this->colorSelectedWindow);
	ImGui::Begin("Properties Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	//TODO
	//TabItem
	if (ImGui::BeginTabBar("Properties", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Images")) {
			ImGui::Text("Data of the Image");
			ImGui::Separator();
			ImGui::Text("Identifier: %d", this->CurrentImage->texture);
			ImGui::Text("Size = %d x %d", this->CurrentImage->width, CurrentImage->height);
			ImGui::Text("Channels: %d", this->CurrentImage->channels);
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
			for (int i = 0; i < this->Images->size(); i++) {
				ImGui::Image((void*)(intptr_t)this->Images->at(i).texture, ImVec2(100 * temp_percentage, 100 * temp_percentage));
				ImGui::SameLine();
				ImGui::Selectable(this->Images->at(i).name.c_str());
				if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
				{
					int n_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
					if (n_next >= 0 && n_next < this->Images->size())
					{
						std::swap(this->Images->at(i), this->Images->at(n_next));
						ImGui::ResetMouseDragDelta();
						*this->CurrentImage = this->Images->front();
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
	ImGui::PushStyleColor(ImGuiCol_WindowBg, this->colorSelectedWindow);
	//Adjust the font size to fit the window
	ImGui::Begin("Info Bar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	//TODO
	{
		//Check first if the mouse is over the Image Window
		if (this->MouseImagePositionX > 0 && this->MouseImagePositionX <= this->CurrentImage->width &&
			this->MouseImagePositionY > 0 && this->MouseImagePositionY <= this->CurrentImage->height) {
			ImGui::Text("Current Pixel: (%d, %d)", this->MouseImagePositionX, this->MouseImagePositionY);
			
			//Read from the texture of opengl and Get the color of the pixel
			ImGui::SameLine();
			int index = ((this->MouseImagePositionY-1) * this->CurrentImage->width + (this->MouseImagePositionX-1)) * this->CurrentImage->channels;
			if (this->CurrentImage->channels == 4) {
				ImGui::Text(" |  Color: (%d, %d, %d, %d)", this->CurrentImage->data[index], this->CurrentImage->data[index + 1], this->CurrentImage->data[index + 2], this->CurrentImage->data[index + 3]);
			}
			else
			{
				ImGui::Text(" |  Color: (%d, %d, %d)", this->CurrentImage->data[index], this->CurrentImage->data[index + 1], this->CurrentImage->data[index + 2]);
			}
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

//######################### EXTERN WINDOWS #########################

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
	//If the mouse is over the image
	if (this->MouseImagePositionX >= 0 && this->MouseImagePositionX <= this->CurrentImage->width &&
		this->MouseImagePositionY >= 0 && this->MouseImagePositionY <= this->CurrentImage->height) {
		//If mouse click or holded
		if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0)) {
			if (this->CurrentTool != -1) {
				workStation.useTool(this->CurrentTool,this->MouseImagePositionX,this->MouseImagePositionY);
				this->UpdateImage();
			}
		}
	}

	//Position of the mouse inside the Image Window compensating the image shift and zoom
	this->MouseImagePositionX = (int)ceil(static_cast<float>((this->io->MousePos.x - this->toolsPanelPixels - this->imageShiftX)) / (float)this->zoom);
	this->MouseImagePositionY = (int)ceil(static_cast<float>((this->io->MousePos.y - this->MenuSizePixels - this->imageShiftY)) / (float)this->zoom);
}
