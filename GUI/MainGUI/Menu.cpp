#include "../feathergui.h"

void FeatherGUI::OpenMenuFunction() {
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
			workStation.combineLayers();
			this->centerImage();
		}
	}
}

void FeatherGUI::SaveMenuFunction(int _type) {
	std::string folderName;
	folderName = browseFolder(0, _type);
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

void FeatherGUI::BuildMenu() {
	//Create a main menu
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu(ICON_FA_FILE_ALT " File")) {
			if (ImGui::MenuItem(ICON_FA_FILE " New", "|Ctrl+N")) {
				this->newImagePopUp = true;
				std::cout << "Opened New Image window Pop Up" << std::endl;
			}
			if (ImGui::MenuItem(ICON_FA_FILE_IMAGE " Open", "|Ctrl+O")) {
				this->OpenMenuFunction();
			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save", "|Ctrl+S")) {
				this->SaveMenuFunction(0);
			}
			if (ImGui::MenuItem(ICON_FA_SAVE " Save As", "|Ctrl+Shift+S")) {
				this->SaveMenuFunction(1);
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FA_WINDOW_CLOSE " Close", "|Alt+F4")) {
				//Free every image
				for (int i = 0; i < this->workStation.ImagesSize(); i++) {
					//Free the image
					glDeleteTextures(1, &this->workStation.getImage(i)->texture);
					//delete Image data
					this->workStation.clearImages();
				}
				this->isOpen = false;
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();
		//Edit
		if (ImGui::BeginMenu(ICON_FA_EDIT " Edit")) {
			//Histogram
			if (ImGui::MenuItem(ICON_FA_IMAGES " Histogram")) {
				//Open a new window
				this->histogramPopUp = true;
				//Calculate histogram
				this->workStation.calculateHistogram(this->workStation.getImageStrP());
				std::cout << "Opened Histogram window Pop Up" << std::endl;
			}
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