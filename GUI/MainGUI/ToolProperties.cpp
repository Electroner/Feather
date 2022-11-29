#include "../feathergui.h"

void FeatherGUI::BuildToolProperties() {
	float temp_radius = static_cast<float>(this->workStation.getToolRadius());

	RGB temp_color_RGB = this->workStation.getToolColor();
	float temp_color[4] = { temp_color_RGB.r,temp_color_RGB.g, temp_color_RGB.b, 1.0F };
	
	RGB temp_secondary_color_RGB = this->workStation.getSecondaryColor();
	float temp_secondary_color[4] = { temp_secondary_color_RGB.r,temp_secondary_color_RGB.g, temp_secondary_color_RGB.b, temp_secondary_color_RGB.delta };

	ImGui::Text("\n");

	//PENCIL
	//Create a ColorEdit3 if the channels are 3
	if (this->workStation.getImageStrP()->channels == 4) {
		ImGui::ColorEdit4("Tool", temp_color);
		temp_color_RGB.r = temp_color[0];
		temp_color_RGB.g = temp_color[1];
		temp_color_RGB.b = temp_color[2];
		temp_color_RGB.delta = temp_color[3];
	}
	else
	{
		ImGui::ColorEdit3("Tool", temp_color);
		temp_color_RGB.r = temp_color[0];
		temp_color_RGB.g = temp_color[1];
		temp_color_RGB.b = temp_color[2];


	}
	this->workStation.setToolColor(temp_color_RGB);
	
	//ERASER
	
	ImGui::ColorEdit4("Eraser", temp_secondary_color);
	temp_secondary_color_RGB.r = temp_secondary_color[0];
	temp_secondary_color_RGB.g = temp_secondary_color[1];
	temp_secondary_color_RGB.b = temp_secondary_color[2];
	temp_secondary_color_RGB.delta = temp_secondary_color[3];

	this->workStation.setSecondaryColor(temp_secondary_color_RGB);

	//Create a SliderFloat for the brush size
	ImGui::SliderFloat("Size", &temp_radius, 0.0F, 100.0F);
	this->workStation.setToolRadius(static_cast<int>(temp_radius));

	switch (this->CurrentTool)
	{
	case 0:
		//PENCIL
		
		break;

	case 1:
		break;

	case 2:
		//ERASER
		break;
		
	case 3:
		break;

	default:
		break;
	}
}