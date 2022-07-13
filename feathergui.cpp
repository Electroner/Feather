#include "feathergui.h"
#include "pathwork.h"
#include "IconsFontAwesome.h"

void FeatherGUI::BuildGUI() {
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
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	
	//Begin("Imagen Displayer");
	//Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / GetIO().Framerate, GetIO().Framerate);
	//static int selector = 0;
	//ImGui::SliderInt(": Seleccion Imagen", &selector, 0, 2);
	//
	//if (Images.empty()) {
	//	if (!loadImage("./resoruces/exampleimages/manzana.png")) {
	//		Text("Error loading image");
	//	}
	//	if (!loadImage("./resoruces/exampleimages/naranja.png")) {
	//		Text("Error loading image");
	//	}
	//	if (!loadImage("./resoruces/exampleimages/nebula.jpg")) {
	//		Text("Error loading image");
	//	}
	//}
	//else 
	//{
	//	CurrentImage = Images.at(selector);
	//}
	//
	////Show CurrentImage data in ImGui
	//if (CurrentImage.loaded) {
	//	Text("------------------------------------------------");
	//	Text("Identifier = %p", CurrentImage.texture);
	//	Text("Size = %d x %d", CurrentImage.width, CurrentImage.height);
	//	Text("Channels: %d", CurrentImage.channels);
	//	Text("Mouse wheel: %f", io->MouseWheel);
	//}

	//if (io->MouseWheel > 0) {	//UP
	//		//increase lineal zoom by 0.1 if this one is less than 3
	//	if (this->zoom > 0.0F) {
	//		this->zoom -= this->zoomIncrement;
	//	}
	//}
	//if (io->MouseWheel < 0) {	//DOWN
	//	//decrease lineal zoom by 0.1 if this one is greater than 0.1
	//	if (this->zoom < 10.0F) {
	//		this->zoom += this->zoomIncrement;
	//	}
	//}
	//
	//ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	//ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	//if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	//if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	//ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	//ImGuiIO& io = ImGui::GetIO();
	//ImDrawList* draw_list = ImGui::GetWindowDrawList();
	//draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	//draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));
	//draw_list->AddImage((void*)(intptr_t)CurrentImage.texture, canvas_p0, canvas_p1, ImVec2(0, 1.0F * this->zoom), ImVec2(1.0F * this->zoom, 0));

	//Show CurrentImage in ImGui
	if(!this->Images.empty())
	if (CurrentImage.data != NULL) {
		ImGui::Image((void*)(intptr_t)CurrentImage.texture, ImVec2(CurrentImage.width * this->zoom, CurrentImage.height * this->zoom));
	}

	ImGui::End();
}

FeatherGUI::FeatherGUI(GLFWwindow* _windowContext, const char* _glsl_version)
{
	this->windowContext = _windowContext;
	
	// Initialize the Feather GUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	this->io = &ImGui::GetIO();
	(void)io;
	//io->Fonts->AddFontDefault();
	this->CurrentFont = io->Fonts->AddFontFromFileTTF("./resoruces/consolas.ttf", 16);
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

	this->zoom = 1;
	this->zoomIncrement = 0.05F;
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