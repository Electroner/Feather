#include "../feathergui.h"

void FeatherGUI::BuildLayers() {
	//LAYER WINDOW
	float temp_percentage = ((float)(static_cast<float>(this->propertiesPanelPixels) / static_cast<float>(this->windowWidth)));
	ImGui::SetNextWindowPos(ImVec2((1.0F - temp_percentage) * this->io->DisplaySize.x, (this->io->DisplaySize.y + this->MenuSizePixels) / 2), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(temp_percentage * this->io->DisplaySize.x, (this->io->DisplaySize.y - this->MenuSizePixels) / 2 - this->infoPanelPixels), ImGuiCond_Always);
	//TODO
	ImGui::Begin("Layers Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);
	if (ImGui::BeginTabBar("Layers", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Layers")) {
			ImGuiStyle& style = ImGui::GetStyle();

			ImVec2 windowPosition = ImGui::GetWindowPos();
			ImVec2 cursorPosition = ImGui::GetCursorPos();

			// this is not a pixel perfect position
			// you can try to make it more accurate by adding some offset
			ImVec2 itemPosition(
				windowPosition.x + cursorPosition.x,
				windowPosition.y + cursorPosition.y - style.ItemSpacing.y
			);

			for (int i = 0; i < this->Images->size(); i++) {
				ImGui::Image((void*)(intptr_t)this->Images->at(i).texture, ImVec2(100 * temp_percentage, 100 * temp_percentage));
				ImGui::SameLine();
				ImGui::Selectable(this->Images->at(i).name.c_str());

				if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
				{
					int n_next = static_cast<int>(floorf((ImGui::GetMousePos().y - itemPosition.y) / 25)); //25 is the height of the item

					if (n_next >= 0 && n_next < this->Images->size())
					{
						std::swap(this->Images->at(i), this->Images->at(n_next));
						workStation.selectFrontImage();
						centerImage();
					}
				}
				ImGui::Separator();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	//TODOEND
	ImGui::End();
	ImGui::PopStyleColor();
}