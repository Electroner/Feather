#include "feathergui.h"
#include "pathwork.h"
#include "IconsFontAwesome.h"

void FeatherGUI::BuildGUI() {
	using namespace ImGui;
	
	//Create a main menu
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu(ICON_FA_FILE_ALT " File")) {
			if (ImGui::MenuItem(ICON_FA_IMAGE " New")) {

			}
			if (ImGui::MenuItem(ICON_FA_FILE " Open")) {
				
			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save")) {
				
			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save As")) {
				
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Close")) {
				this->isOpen = false;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(ICON_FA_EDIT "Edit")) {
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	Begin("Imagen Displayer");
	Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / GetIO().Framerate, GetIO().Framerate);
	static int selector = 0;
	ImGui::SliderInt(": Seleccion Imagen", &selector, 0, 2);
	
	if (Images.empty()) {
		if (!loadImage("./resoruces/exampleimages/manzana.png")) {
			Text("Error loading image");
		}
		if (!loadImage("./resoruces/exampleimages/naranja.png")) {
			Text("Error loading image");
		}
		if (!loadImage("./resoruces/exampleimages/nebula.jpg")) {
			Text("Error loading image");
		}
	}
	else 
	{
		CurrentImage = Images.at(selector);
	}
	
	//Show CurrentImage data in ImGui
	if (CurrentImage.loaded) {
		Text("------------------------------------------------");
		Text("Identifier = %p", CurrentImage.texture);
		Text("Size = %d x %d", CurrentImage.width, CurrentImage.height);
		Text("Channels: %d", CurrentImage.channels);
	}

	//Show CurrentImage in ImGui
	if (CurrentImage.data != NULL) {
		Image((void*)(intptr_t)CurrentImage.texture, ImVec2(CurrentImage.width, CurrentImage.height));
	}
	
	End();
}

FeatherGUI::FeatherGUI(GLFWwindow* _windowContext, const char* _glsl_version)
{
	this->windowContext = _windowContext;
	
	// Initialize the Feather GUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	this->io = &ImGui::GetIO();
	(void)io;
	io->Fonts->AddFontDefault();
	ImGui::StyleColorsDark();
	this->isOpen = true;
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