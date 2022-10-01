#include "../feathergui.h"

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
		if (this->MouseImagePositionX >= 0 && this->MouseImagePositionX < this->CurrentImage->width &&
			this->MouseImagePositionY >= 0 && this->MouseImagePositionY < this->CurrentImage->height) {
			ImGui::Text("Current Pixel: (%d, %d)", this->MouseImagePositionX + 1, this->MouseImagePositionY + 1);

			//Read from the texture of opengl and Get the color of the pixel
			ImGui::SameLine();
			int index = (this->MouseImagePositionY * this->CurrentImage->width + this->MouseImagePositionX) * this->CurrentImage->channels;
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