#include "../feathergui.h"

void FeatherGUI::BuildToolProperties() {
	RGB temp_color_RGB = this->workStation.getToolColor();
	float temp_color[4] = { temp_color_RGB.r,temp_color_RGB.g, temp_color_RGB.b, 1.0F };
	switch (this->CurrentTool)
	{
	case 0:
		//Create a ColorEdit3
		ImGui::ColorEdit3("Color", temp_color);
		temp_color_RGB.r = temp_color[0];
		temp_color_RGB.g = temp_color[1];
		temp_color_RGB.b = temp_color[2];
		this->workStation.setToolColor(temp_color_RGB);
		break;

	default:
		break;
	}
}