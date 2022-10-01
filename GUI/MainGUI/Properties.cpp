#include "../feathergui.h"

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
		ImGui::Separator();
		//If pencil tool is selected
		this->BuildToolProperties();
	}
	ImGui::End();
}