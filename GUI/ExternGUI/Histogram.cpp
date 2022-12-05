#include "../feathergui.h"

void FeatherGUI::BuildHistogram() {
	if (ImGui::Begin(ICON_FA_IMAGE " Histogram", &this->histogramPopUp, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	{
		//If the histogram arrays are not null
		if (this->workStation.getImageStrP()->histogramR == NULL &&
			this->workStation.getImageStrP()->histogramG == NULL &&
			this->workStation.getImageStrP()->histogramB == NULL) {
		}
		else {
			//Draw the histogram	
			ImGui::PlotHistogram("Histogram", this->workStation.getImageStrP()->histogramR, 256, 0, NULL, 0.0f, 1.0F, ImVec2(0, 80));
			ImGui::PlotHistogram("Histogram", this->workStation.getImageStrP()->histogramG, 256, 0, NULL, 0.0f, 1.0F, ImVec2(0, 80));
			ImGui::PlotHistogram("Histogram", this->workStation.getImageStrP()->histogramB, 256, 0, NULL, 0.0f, 1.0F, ImVec2(0, 80));
		}
		ImGui::End();
	}
}
