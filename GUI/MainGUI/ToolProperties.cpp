#include "../feathergui.h"

void FeatherGUI::BuildToolProperties() {
	RGB temp_color_RGB = this->workStation.getToolColor();
	float temp_color[4] = { temp_color_RGB.r,temp_color_RGB.g, temp_color_RGB.b, 1.0F };
	
	float temp_radius = this->workStation.getToolRadius();

	switch (this->CurrentTool)
	{
	case 0:
		//Create a ColorEdit3 if the channels are 3
		if (this->CurrentImage->channels == 4) {
			ImGui::ColorEdit4("Color", temp_color);
			temp_color_RGB.r = temp_color[0];
			temp_color_RGB.g = temp_color[1];
			temp_color_RGB.b = temp_color[2];
			temp_color_RGB.delta = temp_color[3];
		}
		else 
		{
			ImGui::ColorEdit3("Color", temp_color);
			temp_color_RGB.r = temp_color[0];
			temp_color_RGB.g = temp_color[1];
			temp_color_RGB.b = temp_color[2];
		}
		this->workStation.setToolColor(temp_color_RGB);
		
		//Create a SliderFloat for the brush size
		ImGui::SliderFloat("Size", &temp_radius, 0.0F, 100.0F);
		this->workStation.setToolRadius(temp_radius);
		
		break;

	default:
		break;
	}
}