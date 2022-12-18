#include "../feathergui.h"

void FeatherGUI::toolDisplays() {
	//SELECTION
	//if selection is enable draw the selection
	if (this->workStation.getSelectionEnabled()) {
		this->indexMinX = this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMin().first * this->zoom;
		this->indexMinY = this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMin().second * this->zoom;
		this->indexMaxX = this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMax().first * this->zoom;
		this->indexMaxY = this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMax().second * this->zoom;
	
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(indexMinX, indexMinY),
			ImVec2(indexMaxX, indexMaxY),
			IM_COL32(128, 128, 128, 255), 0.0F, 0, 2.0F);

		//draw squares in the corners of the selection
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMinX - this->selectionSquareSize / 2, indexMinY - this->selectionSquareSize / 2),
			ImVec2(indexMinX + this->selectionSquareSize / 2, indexMinY + this->selectionSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMaxX - this->selectionSquareSize / 2, indexMinY - this->selectionSquareSize / 2),
			ImVec2(indexMaxX + this->selectionSquareSize / 2, indexMinY + this->selectionSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMinX - this->selectionSquareSize / 2, indexMaxY - this->selectionSquareSize / 2),
			ImVec2(indexMinX + this->selectionSquareSize / 2, indexMaxY + this->selectionSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMaxX - this->selectionSquareSize / 2, indexMaxY - this->selectionSquareSize / 2),
			ImVec2(indexMaxX + this->selectionSquareSize / 2, indexMaxY + this->selectionSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
	
		float indexMiddleX = this->imageShiftX + this->toolsPanelPixels + (this->workStation.getSelectionMin().first + this->workStation.getSelectionMax().first) / 2 * this->zoom;
		float indexMiddleY = this->imageShiftY + this->MenuSizePixels + (this->workStation.getSelectionMin().second + this->workStation.getSelectionMax().second) / 2 * this->zoom;
		//Draw squares between the corners of the selection
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMinX - this->selectionSquareSize / 2, indexMiddleY - this->selectionSquareSize / 2),
			ImVec2(indexMinX + this->selectionSquareSize / 2, indexMiddleY + this->selectionSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMaxX - this->selectionSquareSize / 2, indexMiddleY - this->selectionSquareSize / 2),
			ImVec2(indexMaxX + this->selectionSquareSize / 2, indexMiddleY + this->selectionSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMiddleX - this->selectionSquareSize / 2, indexMinY - this->selectionSquareSize / 2),
			ImVec2(indexMiddleX + this->selectionSquareSize / 2, indexMinY + this->selectionSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMiddleX - this->selectionSquareSize / 2, indexMaxY - this->selectionSquareSize / 2),
			ImVec2(indexMiddleX + this->selectionSquareSize / 2, indexMaxY + this->selectionSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
	}
	
	//SELECTION SCALE
	//if selection is enable draw the selection
	if (this->workStation.getSelectionScaleEnabled()) {
		this->indexScaleMinX = this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionScaleMin().first * this->zoom;
		this->indexScaleMinY = this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionScaleMin().second * this->zoom;
		this->indexScaleMaxX = this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionScaleMax().first * this->zoom;
		this->indexScaleMaxY = this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionScaleMax().second * this->zoom;

	
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(indexScaleMinX, indexScaleMinY),
			ImVec2(indexScaleMaxX, indexScaleMaxY),
			IM_COL32(128, 128, 128, 255), 0.0F, 0, 2.0F);
	
		//draw squares in the corners of the selection
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexScaleMinX - this->selectionScaleSquareSize / 2, indexScaleMinY - this->selectionScaleSquareSize / 2),
			ImVec2(indexScaleMinX + this->selectionScaleSquareSize / 2, indexScaleMinY + this->selectionScaleSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexScaleMaxX - this->selectionScaleSquareSize / 2, indexScaleMinY - this->selectionScaleSquareSize / 2),
			ImVec2(indexScaleMaxX + this->selectionScaleSquareSize / 2, indexScaleMinY + this->selectionScaleSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexScaleMinX - this->selectionScaleSquareSize / 2, indexScaleMaxY - this->selectionScaleSquareSize / 2),
			ImVec2(indexScaleMinX + this->selectionScaleSquareSize / 2, indexScaleMaxY + this->selectionScaleSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexScaleMaxX - this->selectionScaleSquareSize / 2, indexScaleMaxY - this->selectionScaleSquareSize / 2),
			ImVec2(indexScaleMaxX + this->selectionScaleSquareSize / 2, indexScaleMaxY + this->selectionScaleSquareSize / 2),
			IM_COL32(128, 128, 128, 255));

	
		float indexMiddleScaleX = this->imageShiftX + this->toolsPanelPixels + (this->workStation.getSelectionScaleMin().first + this->workStation.getSelectionScaleMax().first) / 2 * this->zoom;
		float indexMiddleScaleY = this->imageShiftY + this->MenuSizePixels + (this->workStation.getSelectionScaleMin().second + this->workStation.getSelectionScaleMax().second) / 2 * this->zoom;
		//Draw squares between the corners of the selection
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexScaleMinX - this->selectionScaleSquareSize / 2, indexMiddleScaleY - this->selectionScaleSquareSize / 2),
			ImVec2(indexScaleMinX + this->selectionScaleSquareSize / 2, indexMiddleScaleY + this->selectionScaleSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexScaleMaxX - this->selectionScaleSquareSize / 2, indexMiddleScaleY - this->selectionScaleSquareSize / 2),
			ImVec2(indexScaleMaxX + this->selectionScaleSquareSize / 2, indexMiddleScaleY + this->selectionScaleSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMiddleScaleX - this->selectionScaleSquareSize / 2, indexScaleMinY - this->selectionScaleSquareSize / 2),
			ImVec2(indexMiddleScaleX + this->selectionScaleSquareSize / 2, indexScaleMinY + this->selectionScaleSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(indexMiddleScaleX - this->selectionScaleSquareSize / 2, indexScaleMaxY - this->selectionScaleSquareSize / 2),
			ImVec2(indexMiddleScaleX + this->selectionScaleSquareSize / 2, indexScaleMaxY + this->selectionScaleSquareSize / 2),
			IM_COL32(128, 128, 128, 255));
	}
}