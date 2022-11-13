#include "../feathergui.h"

void FeatherGUI::toolDisplays() {
	//SELECTION
	//if selection is enable draw the selection
	float indexMinX = this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMin().first * this->zoom;
	float indexMinY = this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMin().second * this->zoom;
	float indexMaxX = this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMax().first * this->zoom;
	float indexMaxY = this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMax().second * this->zoom;
	
	if (this->workStation.getSelectionEnabled()) {
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(indexMinX, indexMinY),
			ImVec2(indexMaxX, indexMaxY),
			IM_COL32(128, 128, 128, 255), 0.0F, 0, 2.0F);
	}

	//draw squares in the corners of the selection
	int square_size = 8;
	if (this->workStation.getSelectionEnabled()) {
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMinX - square_size / 2, indexMinY - square_size / 2),
			ImVec2(indexMinX + square_size / 2, indexMinY + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMaxX - square_size / 2, indexMinY - square_size / 2),
			ImVec2(indexMaxX + square_size / 2, indexMinY + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMinX - square_size / 2, indexMaxY - square_size / 2),
			ImVec2(indexMinX + square_size / 2, indexMaxY + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMaxX - square_size / 2, indexMaxY - square_size / 2),
			ImVec2(indexMaxX + square_size / 2, indexMaxY + square_size / 2),
			IM_COL32(128, 128, 128, 255));
	}
	
	float indexMiddleX = this->imageShiftX + this->toolsPanelPixels + (this->workStation.getSelectionMin().first + this->workStation.getSelectionMax().first) / 2 * this->zoom;
	float indexMiddleY = this->imageShiftY + this->MenuSizePixels + (this->workStation.getSelectionMin().second + this->workStation.getSelectionMax().second) / 2 * this->zoom;
	//Draw squares between the corners of the selection
	if (this->workStation.getSelectionEnabled()) {
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMinX - square_size / 2, indexMiddleY - square_size / 2),
			ImVec2(indexMinX + square_size / 2, indexMiddleY + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMaxX - square_size / 2, indexMiddleY - square_size / 2),
			ImVec2(indexMaxX + square_size / 2, indexMiddleY + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMiddleX - square_size / 2, indexMinY - square_size / 2),
			ImVec2(indexMiddleX + square_size / 2, indexMinY + square_size / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMiddleX - square_size / 2, indexMaxY - square_size / 2),
			ImVec2(indexMiddleX + square_size / 2, indexMaxY + square_size / 2),
			IM_COL32(128, 128, 128, 255));
	}
}