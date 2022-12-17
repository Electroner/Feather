#include "../feathergui.h"

void FeatherGUI::BuildConfigMenu() {
	//PLACEMENT CONFIG WINDOW
	ImGui::Begin("Placement Config", &this->placementConfig, ImGuiWindowFlags_AlwaysAutoResize);
	//Create a slider for the toolsPanelPercentage
	ImGui::SliderInt("Tools Panel Pixels", &this->toolsPanelPixels, 32, 128);
	//Create a slider for the propertiesPanelPercentage
	ImGui::SliderInt("Properties Panel Pixels", &this->propertiesPanelPixels, 128, 512);

	float color[4] = { static_cast<float>(GetBackGroundColor().r / 255.0F),  static_cast<float>(GetBackGroundColor().g / 255.0F),  static_cast<float>(GetBackGroundColor().b / 255.0F), 1.0F };
	//Change background color
	ImGui::ColorEdit3("Background Color", color);
	SetBackGroundColor((unsigned char)(color[0] * 255), (unsigned char)(color[1] * 255), (unsigned char)(color[2] * 255));

	//Separator
	ImGui::Separator();
	ImGui::Text("Rounding Options");
	ImGui::SliderFloat("windowRounding", &this->windowRounding, 0.0F, 10.0F);
	ImGui::SameLine();
	HelpMarker("Disable rounding by setting this to 0.0");
	//Disable options rounding
	ImGui::Checkbox("Options Rounding", &this->disableOptionsRounding);
	if (this->disableOptionsRounding) {
		ImGui::GetStyle().FrameRounding = 0.0F;
	}
	else
	{
		ImGui::GetStyle().FrameRounding = 5.0F;
	}
	//Enable/disable sync
	bool tempsync = this->Vsync;
	ImGui::Checkbox("Vertical Sync", &tempsync);
	if (tempsync != this->Vsync) {
		this->SetSync(tempsync);
	}
	ImGui::End();
}