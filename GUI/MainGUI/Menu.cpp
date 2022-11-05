#include "../feathergui.h"

void FeatherGUI::BuildMenu() {
	//Create a main menu
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu(ICON_FA_FILE_ALT " File")) {
			if (ImGui::MenuItem(ICON_FA_FILE " New", "|Ctrl+N")) {
				this->newImagePopUp = true;
				std::cout << "Opened New Image window Pop Up" << std::endl;
			}
			if (ImGui::MenuItem(ICON_FA_FILE_IMAGE " Open", "|Ctrl+O")) {
				std::string filename;
				filename = browseFile(0);
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
						workStation.selectFrontImage();
						this->centerImage();
					}
				}
			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save", "|Ctrl+S")) {
				std::string folderName;
				folderName = browseFolder(0, 0);
				//If the foldername doesnt have ":" error
				if (folderName.find(':') == std::string::npos) {
					std::cout << "No folder selected" << std::endl;
				}
				else
				{
					if (!saveImage(folderName)) {
						std::cout << "Error saving image" << std::endl;
					}
					else {
						std::cout << "Image saved" << std::endl;
					}
				}
			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save As", "|Ctrl+Shift+S")) {
				std::string folderName;
				folderName = browseFolder(0, 1);
				//If the foldername doesnt have ":" error
				if (folderName.find(':') == std::string::npos) {
					std::cout << "No folder selected" << std::endl;
				}
				else
				{
					if (!saveImage(folderName)) {
						std::cout << "Error saving image" << std::endl;
					}
					else {
						std::cout << "Image saved" << std::endl;
					}
				}
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