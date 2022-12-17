#include "../feathergui.h"

void FeatherGUI::BuildToolProperties() {
	float temp_radius = static_cast<float>(this->workStation.getToolRadius());

	RGB temp_color_RGB = this->workStation.getToolColor();
	float temp_color[4] = { 
		static_cast<float>(temp_color_RGB.r / 255.0F),
		static_cast<float>(temp_color_RGB.g / 255.0F),
		static_cast<float>(temp_color_RGB.b / 255.0F),
		1.0F };

	RGB temp_secondary_color_RGB = this->workStation.getSecondaryColor();
	float temp_secondary_color[4] = { 
		static_cast<float>(temp_secondary_color_RGB.r / 255.0F),
		static_cast<float>(temp_secondary_color_RGB.g / 255.0F),
		static_cast<float>(temp_secondary_color_RGB.b / 255.0F),
		static_cast<float>(temp_secondary_color_RGB.delta / 255.0F) };

	int temp_tolerance = this->workStation.getTolerance();

	ImGui::Text("\n");

	//Tool Previsualization
	ImGui::Checkbox("Tool Previsualization", &this->ToolPrevisualization);

	//PENCIL
	//Create a ColorEdit3 if the channels are 3
	ImGui::ColorEdit4("Tool", temp_color);
	temp_color_RGB.r = static_cast<unsigned char>(temp_color[0] * 255);
	temp_color_RGB.g = static_cast<unsigned char>(temp_color[1] * 255);
	temp_color_RGB.b = static_cast<unsigned char>(temp_color[2] * 255);
	temp_color_RGB.delta = static_cast<unsigned char>(temp_color[3] * 255);

	this->workStation.setToolColor(temp_color_RGB);

	//ERASER

	ImGui::ColorEdit4("Eraser", temp_secondary_color);
	temp_secondary_color_RGB.r = static_cast<unsigned char>(temp_secondary_color[0] * 255);
	temp_secondary_color_RGB.g = static_cast<unsigned char>(temp_secondary_color[1] * 255);
	temp_secondary_color_RGB.b = static_cast<unsigned char>(temp_secondary_color[2] * 255);
	temp_secondary_color_RGB.delta = static_cast<unsigned char>(temp_secondary_color[3] * 255);

	this->workStation.setSecondaryColor(temp_secondary_color_RGB);

	//Create a SliderFloat for the brush size
	ImGui::SliderFloat("Size", &temp_radius, 0.0F, 100.0F);
	this->workStation.setToolRadius(static_cast<int>(temp_radius));

	switch (this->CurrentTool)
	{
	case TOOL_PENCIL:
		if (ToolPrevisualization) {
			//If the cursor is inside the image
			if (this->MouseImagePositionX >= 0 && this->MouseImagePositionY >= 0 &&
				this->MouseImagePositionX < this->workStation.getImageStrP()->width &&
				this->MouseImagePositionY < this->workStation.getImageStrP()->height) {
				//Create a cross cursor with the size of the brush radius and the color of the brush
				ImGui::GetForegroundDrawList()->AddCircleFilled(
					ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y),
					temp_radius * this->zoom,
					ImGui::ColorConvertFloat4ToU32(ImVec4(
						static_cast<float>(temp_color_RGB.r / 255.0F),
						static_cast<float>(temp_color_RGB.g / 255.0F),
						static_cast<float>(temp_color_RGB.b / 255.0F),
						0.5)),
					0);
			}
		}
		break;

	case TOOL_BRUSH:
		if (ToolPrevisualization) {
			//If the cursor is inside the image
			if (this->MouseImagePositionX >= 0 && this->MouseImagePositionY >= 0 &&
				this->MouseImagePositionX < this->workStation.getImageStrP()->width &&
				this->MouseImagePositionY < this->workStation.getImageStrP()->height) {
				//Create a cross cursor with the size of the brush radius and the color of the brush
				ImGui::GetForegroundDrawList()->AddCircleFilled(
					ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y),
					temp_radius * this->zoom,
					ImGui::ColorConvertFloat4ToU32(ImVec4(
						static_cast<float>(temp_color_RGB.r / 255.0F),
						static_cast<float>(temp_color_RGB.g / 255.0F),
						static_cast<float>(temp_color_RGB.b / 255.0F),
						0.5)),
					0);
			}
		}
		break;

	case TOOL_ERASER:
		if (ToolPrevisualization) {
			//If the cursor is inside the image
			if (this->MouseImagePositionX >= 0 && this->MouseImagePositionY >= 0 &&
				this->MouseImagePositionX < this->workStation.getImageStrP()->width &&
				this->MouseImagePositionY < this->workStation.getImageStrP()->height) {
				//Create a cross cursor with the size of the brush radius and the color of the brush
				ImGui::GetForegroundDrawList()->AddCircleFilled(
					ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y),
					temp_radius * this->zoom,
					ImGui::ColorConvertFloat4ToU32(ImVec4(
						static_cast<float>(temp_secondary_color_RGB.r / 255.0F),
						static_cast<float>(temp_secondary_color_RGB.g / 255.0F),
						static_cast<float>(temp_secondary_color_RGB.b / 255.0F),
						0.5)),
					0);
			}
		}
		break;

	case TOOL_COLORPICKER:
		break;

	case TOOL_SELECTION:
		break;

	case TOOL_SCALE:
		break;

	case TOOL_BUCKET:
		//Tolerance selector
		ImGui::SliderInt("Tolerance", &temp_tolerance, 0, 100);
		this->workStation.setTolerance(temp_tolerance);
		break;

	case TOOL_TEXT:
		break;

	default:
		this->CursorEnabled = true;
		break;
	}
}