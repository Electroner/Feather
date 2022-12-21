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
}

void FeatherGUI::selectionUpdate() {
	//If mouse is over the left corner of the selection with the radius of half of selectionSquareSize
	if (abs(this->MouseImagePositionX - workStation.getSelectionMin().first) < this->selectionSquareSize || topLeft || bottomLeft || middleLeft) {
		//Lateral Left vertical line
		if (abs(this->MouseImagePositionY - workStation.getSelectionMin().second) < this->selectionSquareSize || topLeft) {
			//Top horizontal line
			//Top Left corner
			//If mouse click and holded change the IndexMin to the mouse position
			if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
				this->workStation.setSelectionMin(std::make_pair(this->MouseImagePositionX, this->MouseImagePositionY));
				topLeft = true;
			}
		}
		else if (abs(this->MouseImagePositionY - this->workStation.getSelectionMax().second) < this->selectionSquareSize || bottomLeft) {
			//Bottom horizontal line
			//Bottom Left corner
			//If mouse click and holded change the IndexMin to the mouse position
			if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
				//In this case we need to change the IndexMin and IndexMax
				this->workStation.setSelectionMin(std::make_pair(this->MouseImagePositionX, this->workStation.getSelectionMin().second));
				this->workStation.setSelectionMax(std::make_pair(this->workStation.getSelectionMax().first, this->MouseImagePositionY));
				bottomLeft = true;
			}
		}
	}
	//If mouse is over the right corner of the selection with the radius of half of selectionSquareSize
	else if (abs(this->MouseImagePositionX - this->workStation.getSelectionMax().first) < this->selectionSquareSize || topRight || bottomRight) {
		//Lateral Right vertical line
		if (abs(this->MouseImagePositionY - this->workStation.getSelectionMin().second) < this->selectionSquareSize || topRight) {
			//Top horizontal line
			//Top Right corner
			//If mouse click and holded change the IndexMax to the mouse position
			if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
				//In this case we need to change the IndexMin and IndexMax
				this->workStation.setSelectionMin(std::make_pair(this->workStation.getSelectionMin().first, this->MouseImagePositionY));
				this->workStation.setSelectionMax(std::make_pair(this->MouseImagePositionX, this->workStation.getSelectionMax().second));
				topRight = true;
			}
		}
		else if (abs(this->MouseImagePositionY - this->workStation.getSelectionMax().second) < this->selectionSquareSize || bottomRight) {
			//Bottom horizontal line
			//Bottom Right corner
			//If mouse click and holded change the IndexMax to the mouse position
			if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
				this->workStation.setSelectionMax(std::make_pair(this->MouseImagePositionX, this->MouseImagePositionY));
				bottomRight = true;
			}
		}
	}
	//middle Top
	if (abs(this->MouseImagePositionY - this->workStation.getSelectionMin().second) < this->selectionSquareSize || middleTop) {
		//Top horizontal line
		//If mouse click and holded change the IndexMin to the mouse position
		if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
			//In this case we need to change the IndexMin and IndexMax
			this->workStation.setSelectionMin(std::make_pair(this->workStation.getSelectionMin().first, this->MouseImagePositionY));
			this->workStation.setSelectionMax(std::make_pair(this->workStation.getSelectionMax().first, this->workStation.getSelectionMax().second));
			middleTop = true;
		}
	}
	//middle Bottom
	else if (abs(this->MouseImagePositionY - this->workStation.getSelectionMax().second) < this->selectionSquareSize || middleBottom) {
		//Bottom horizontal line
		//If mouse click and holded change the IndexMax to the mouse position
		if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
			this->workStation.setSelectionMax(std::make_pair(this->workStation.getSelectionMax().first, this->MouseImagePositionY));
			middleBottom = true;
		}
	}
	//Middle horizontal left
	else if (abs(this->MouseImagePositionX - this->workStation.getSelectionMin().first) < this->selectionSquareSize || middleLeft) {
		//Lateral Left vertical line
		//If mouse click and holded change the IndexMin to the mouse position
		if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
			this->workStation.setSelectionMin(std::make_pair(this->MouseImagePositionX, this->workStation.getSelectionMin().second));
			this->workStation.setSelectionMax(std::make_pair(this->workStation.getSelectionMax().first, this->workStation.getSelectionMax().second));
			middleLeft = true;
		}
	}
	//Middle horizontal right
	else if (abs(this->MouseImagePositionX - this->workStation.getSelectionMax().first) < this->selectionSquareSize || middleRight) {
		//Lateral Right vertical line
		//If mouse click and holded change the IndexMax to the mouse position
		if (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0) || ImGui::IsMouseDown(0)) {
			this->workStation.setSelectionMax(std::make_pair(this->MouseImagePositionX, this->workStation.getSelectionMax().second));
			middleRight = true;
		}
	}
}
