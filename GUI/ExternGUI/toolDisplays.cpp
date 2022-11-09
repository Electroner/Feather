#include "../feathergui.h"

void FeatherGUI::toolDisplays() {
	//SELECTION
	//if selection is enable draw the selection
	if (this->workStation.getSelectionEnabled()) {
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMin().first * this->zoom, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMin().second * this->zoom),
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMax().first * this->zoom, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMax().second * this->zoom),
			IM_COL32(128, 128, 128, 255), 0.0F, 0, 2.0F);
	}

	//draw squares in the corners of the selection
	int square_size = 8;
	if (this->workStation.getSelectionEnabled()) {
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMin().first * this->zoom - square_size / 2, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMin().second * this->zoom - square_size / 2),
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMin().first * this->zoom + square_size / 2, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMin().second * this->zoom + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMax().first * this->zoom - square_size / 2, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMin().second * this->zoom - square_size / 2),
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMax().first * this->zoom + square_size / 2, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMin().second * this->zoom + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMin().first * this->zoom - square_size / 2, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMax().second * this->zoom - square_size / 2),
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMin().first * this->zoom + square_size / 2, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMax().second * this->zoom + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMax().first * this->zoom - square_size / 2, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMax().second * this->zoom - square_size / 2),
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMax().first * this->zoom + square_size / 2, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMax().second * this->zoom + square_size / 2),
			IM_COL32(128, 128, 128, 255));
	}
}