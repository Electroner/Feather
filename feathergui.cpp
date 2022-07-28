#include "feathergui.h"
#include "pathwork.h"
#include "IconsFontAwesome.h"

void FeatherGUI::BuildGUI() {
	glfwGetWindowSize(this->windowContext, &this->windowWidth, &this->windowHeight);
	
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
			if (ImGui::MenuItem( " GUI Placements", "|Ctrl+Alt+D")) {
				std::cout << "Placement GUI opened" << std::endl;
				this->placementConfig = true;
			}
			if (ImGui::MenuItem( " GUI Colors", "|Ctrl+Alt+G")) {

			}
			if (ImGui::MenuItem( " GUI Fonts", "|Ctrl+Alt+F")) {

			}
			if (ImGui::MenuItem(" Open Debug Log", "|Ctrl+Alt+C")) {
				this->debugConsole = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//TOOLS WINDOW
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(this->GetBackGroundColor().r - 0.05F, this->GetBackGroundColor().g - 0.05F, this->GetBackGroundColor().b - 0.05F, 1.0f));
	//Create a window on the left maximized that occupies toolsPanelPixels of the space.
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float)(static_cast<float>(this->toolsPanelPixels) / static_cast<float>(this->windowWidth)) * io->DisplaySize.x, io->DisplaySize.y), ImGuiCond_Always);
	ImGui::Begin("Tools Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	//TODO
	ImGui::PopStyleColor();
	ImGui::End();
	
	//MAIN SEPARATOR
	ImGui::Separator();

	//LOADING IMAGES
	if (Images.empty()) {
		if (!loadImage("./resoruces/exampleimages/manzana.png")) {
			std::cout << "Error Loading the Image manzana.png" << std::endl;
		}
		if (!loadImage("./resoruces/exampleimages/naranja.png")) {
			std::cout << "Error Loading the Image naranja.png" << std::endl;
		}
		CurrentImage = Images.back();
		std::cout << "Current Image Properties: " << std::endl;
		std::cout << "Identifier: " << CurrentImage.texture << std::endl;
		std::cout << "Size = " << CurrentImage.width << "x" << CurrentImage.height << std::endl;
		std::cout << "Channels: " << CurrentImage.channels << std::endl;
	}

	//IMAGE WINDOW
	ImGui::SetNextWindowPos(ImVec2((float)(static_cast<float>(this->toolsPanelPixels) / static_cast<float>(this->windowWidth)) * io->DisplaySize.x, this->MenuSizePixels), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x, io->DisplaySize.y), ImGuiCond_Always);
	ImGui::Begin("Image Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);
	//Move the image
	ImGui::SetCursorPos(ImVec2(this->imageShiftX, this->imageShiftY));
	//TODO
	//Draw Image
	if (!this->Images.empty()) {
		if (CurrentImage.data != NULL) {
			ImGui::Image((void*)(intptr_t)CurrentImage.texture, ImVec2(CurrentImage.width * this->zoom, CurrentImage.height * this->zoom));
		}
	}
	ImGui::End();

	//MAIN SEPARATOR
	ImGui::Separator();

	//PROPERTIES WINDOW
	float temp_percentage = (1.0F - (float)(static_cast<float>(this->propertiesPanelPixels) / static_cast<float>(this->windowWidth)));
	ImGui::SetNextWindowPos(ImVec2(temp_percentage * this->io->DisplaySize.x, this->MenuSizePixels), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(temp_percentage * this->io->DisplaySize.x, this->io->DisplaySize.y), ImGuiCond_Always);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(this->GetBackGroundColor().r - 0.05F, this->GetBackGroundColor().g - 0.05F, this->GetBackGroundColor().b - 0.05F, 1.0f));
	ImGui::Begin("Properties Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	//TODO
	ImGui::Text("Properties");
	ImGui::PopStyleColor();
	ImGui::End();
	

	//--------------------EXTERN WINDOWS--------------------

	//PLACEMENT CONFIG WINDOW
	if (this->placementConfig) {
		ImGui::Begin("Placement Config", &this->placementConfig);
		//Create a slider for the toolsPanelPercentage
		ImGui::SliderInt("Tools Panel Pixels", &this->toolsPanelPixels, 32, 128);

		float color[4]= {GetBackGroundColor().r,GetBackGroundColor().g, GetBackGroundColor().b, 1.0F};
		//Change background color
		ImGui::ColorEdit3("Background Color", color);
		SetBackGroundColor(color[0], color[1], color[2]);

		//Disable windows rounding
		ImGui::Checkbox("Button Rounding", &this->disableWindowsRounding);
		if (this->disableWindowsRounding) {
			ImGui::GetStyle().FrameRounding = 0.0F;
		}
		else 
		{
			ImGui::GetStyle().FrameRounding = 5.0F;
		}
		ImGui::End();
	}

	//DEBUG CONSOLE WINDOW
	if (this->debugConsole) {
		ImGui::Begin("Debug Console", &this->debugConsole, ImGuiWindowFlags_NoCollapse);
		//FPS
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//Print the mouse
		ImGui::Text("Mouse Position: (%d, %d)", this->MouseImagePositionX, this->MouseImagePositionY);
		//Slider int
		ImGui::Text("Movement X: %d", this->imageShiftX);
		ImGui::Text("Movement Y: %d", this->imageShiftY);

		ImGui::Separator();
		ImGui::Text(ss.str().c_str());
		ImGui::End();
	}

	//--------------------FUNTIONS--------------------

	//Dragging the Image over the window
	if (ImGui::IsMouseDown(1)) {
		this->imageShiftX += (this->io->MouseDelta.x);
		this->imageShiftY += (this->io->MouseDelta.y);
	}

	//Double click set zoom to 1.0F and the image shift to the center of the Image Window
	if (ImGui::IsMouseDoubleClicked(1)) {
		this->zoom = 1.0F;
		//Center X is (1-(x+z))/2+x, where x is toolsPanelPixels and z is propertiesPanelPixels, substract the size of the image
		this->imageShiftX = (1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x / 2.0F - (CurrentImage.width * this->zoom / 2.0F);
		//Center Y is the size of the screen minus the size of the image divided by 2
		this->imageShiftY = io->DisplaySize.y / 2.0F - (CurrentImage.height * this->zoom / 2.0F);
	}
	
	//UP
	if (io->MouseWheel > 0) {
		//increase lineal zoom by 0.1 if this one is less than 3
		if (this->zoom < 10.0F) {
			this->zoom += this->zoomIncrement;
		}
	}
	//DOWN
	if (io->MouseWheel < 0) {
		//decrease lineal zoom by 0.1 if this one is greater than 0.1
		if (this->zoom > 0.0F) {
			this->zoom -= this->zoomIncrement;
		}
	}

	//MOUSE
	//set mouse location inside the image window
	//Position - size of the tool panel and menu
	this->MouseImagePositionX = this->io->MousePos.x - this->toolsPanelPixels - this->SeparatorSizePixels;
	this->MouseImagePositionY = this->io->MousePos.y - this->MenuSizePixels - this->SeparatorSizePixels;
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
	this->debugConsole = false;
	this->placementConfig = false;
	this->disableWindowsRounding = true;

	//Fonts and Text
	this->iconSize = 12.0;

	//Adding Icons
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io->Fonts->AddFontFromFileTTF("./resoruces/icon_regular.ttf", this->getIconSize(), &icons_config, icons_ranges);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_windowContext, true);
	ImGui_ImplOpenGL3_Init(_glsl_version);
	
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
	this->imageShiftX = 8;
	this->imageShiftY = 8;

	//GUI PLACEMENTS
	this->toolsPanelPixels = 32;
	this->propertiesPanelPixels = 256;
	
}

FeatherGUI::~FeatherGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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
	unloadedImage.data = stbi_load(_path.c_str(), &unloadedImage.width, &unloadedImage.height, &unloadedImage.channels, 3);
	if (!unloadedImage.data) {
		fprintf(stderr, "Cannot load image '%s'\n", _path.c_str());
		unloadedImage.loaded = true;
		return false;
	}

	// Create a OpenGL texture identifier and binding
	glGenTextures(1, &unloadedImage.texture);
	glBindTexture(GL_TEXTURE_2D, unloadedImage.texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
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

bool FeatherGUI::IsOpened() {
	return this->isOpen;
}

void FeatherGUI::setIconSize(float _size) {
	this->iconSize = _size;
}

float FeatherGUI::getIconSize() {
	return this->iconSize;
}
