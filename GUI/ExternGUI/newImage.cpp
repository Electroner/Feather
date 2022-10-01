#include "../feathergui.h"

void FeatherGUI::newImage() {

	//make buttons, sliders, radiobuttons white
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25F, 0.25F, 0.25F, 1.20F));
	//Make overbutton selection white
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75F, 0.75F, 0.75F, 1.20F));
	//Make textbox gray
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25F, 0.25F, 0.25F, 1.20F));

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	//Create a new window to ask for the new image size and the new image name
	if (ImGui::Begin(ICON_FA_IMAGE " New Image", &this->newImagePopUp, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Image Name", this->newImageName, 100);
		ImGui::InputInt("Image Width", &this->newImageWidth);
		ImGui::InputInt("Image Height", &this->newImageHeight);
		ImGui::Checkbox("Transparency", &this->newImageTransparency);

		//If the user press the OK button
		if (ImGui::Button("Create Image")) {
			//check if the image size is bigger than 0
			if (this->newImageWidth > 0 && this->newImageHeight > 0 && this->newImageName[0] != '\0') {
				ImageStr blank;
				//Create a new image with the given parameters
				blank.name = this->newImageName;
				blank.width = this->newImageWidth;
				blank.height = this->newImageHeight;
				blank.extension = "png";

				if (this->newImageTransparency)
				{
					blank.channels = 4;
				}
				else
				{
					blank.channels = 3;
				}

				blank.data = new unsigned char[blank.width * blank.height * blank.channels];
				//For every pixel set it white
				for (int i = 0; i < blank.width * blank.height * blank.channels; i++) {
					blank.data[i] = 255;
				}

				//Enable transparency
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				// Create a OpenGL texture identifier and binding
				glGenTextures(1, &blank.texture);
				glBindTexture(GL_TEXTURE_2D, blank.texture);

				// Setup filtering parameters for display
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
				glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, blank.width, blank.height, 0, GL_RGB, GL_UNSIGNED_BYTE, blank.data);

				//Add the new image to the image list
				this->Images->push_back(blank);
				//Set the new image as the current image
				workStation.selectFrontImage();
				this->centerImage();

				*this->newImageName = {};
				//Close the window
				this->newImagePopUp = false;
			}
			else
			{
				this->errorWindowCreateImage = true;
			}
		}
		//Show Buttons on the same line
		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			*this->newImageName = {};
			this->newImagePopUp = false;
		}

		ImGui::End();
	}

	//Show Error Windows in case of error
	this->ErrorWindowCreateImage();

	//Pop style
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}