#include "../feathergui.h"

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
	for (int i = 0; i < this->toolsIcons.size() / 2; i++) {
		temp_i = i;
		if (this->whiteIcons) {
			temp_i = i;
		}
		else
		{
			temp_i = static_cast<int>(i + this->toolsIcons.size() / 2);
		}
		//Select the color for selected icon
		if (this->CurrentTool == i)
		{
			colorActualButton = this->colorSelectedTool;
			if (this->whiteIcons)
			{
				colorActualButton = this->colorSelectedTool;
				temp_i = static_cast<int>(i + this->toolsIcons.size() / 2);
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
		if (ImGui::ImageButton((void*)(intptr_t)this->toolsIcons[temp_i].texture, ImVec2(static_cast<float>(this->toolsPanelPixels / 2), static_cast<float>(this->toolsPanelPixels / 2)), ImVec2(0, 0), ImVec2(1, 1), 0, colorActualButton)) {
			//create a floatingtext with index
			if (this->CurrentTool == i)
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
}