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
		if (this->MouseImagePositionX >= 0 && this->MouseImagePositionX < this->workStation.getImageStrP()->width &&
			this->MouseImagePositionY >= 0 && this->MouseImagePositionY < this->workStation.getImageStrP()->height) {
			ImGui::Text("Current Pixel: (%d, %d)", this->MouseImagePositionX + 1, this->MouseImagePositionY + 1);

			//Read from the texture of opengl and Get the color of the pixel
			ImGui::SameLine();
			int index = (this->MouseImagePositionY * this->workStation.getImageStrP()->width + this->MouseImagePositionX) * this->workStation.getImageStrP()->channels;
			if (this->workStation.getImageStrP()->channels == 4) {
				ImGui::Text(" |  Color: (%d, %d, %d, %d)", this->workStation.getImageStrP()->data[index], this->workStation.getImageStrP()->data[index + 1], this->workStation.getImageStrP()->data[index + 2], this->workStation.getImageStrP()->data[index + 3]);
			}
			else
			{
				ImGui::Text(" |  Color: (%d, %d, %d)", this->workStation.getImageStrP()->data[index], this->workStation.getImageStrP()->data[index + 1], this->workStation.getImageStrP()->data[index + 2]);
			}
		}
		else
		{
			ImGui::Text("Current Pixel: (-,-)");
		}
		//Check if selection is enabled
		if (this->workStation.getSelectionEnabled()) {
			std::pair<int, int> selectionMin = this->workStation.getSelectionMin();
			std::pair<int, int> selectionMax = this->workStation.getSelectionMax();
			ImGui::SameLine();
			ImGui::Text(" |  Selection: (%d, %d) to (%d, %d)  |  Selection Size: (%d, %d)", 
				selectionMin.first, selectionMin.second, 
				selectionMax.first, selectionMax.second, 
				(selectionMax.first - selectionMin.first),
				(selectionMax.second - selectionMin.second));
		}
		//Print the zoom on the same line
		ImGui::SameLine();
		ImGui::Text(" |  Zoom: %.2f", this->zoom);
	}
	//TODOEND
	ImGui::End();
	ImGui::PopStyleColor();
}