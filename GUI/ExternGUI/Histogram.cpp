#include "../feathergui.h"

void FeatherGUI::BuildHistogram() {
	if (this->workStation.getImageStrP()->histogramB == NULL || this->workStation.getImageStrP()->histogramG == NULL || this->workStation.getImageStrP()->histogramR == NULL) {
		this->workStation.calculateHistogram(this->workStation.getImageStrP());
	}
	if (this->workStation.getSizeImages() > 0) {
		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(this->io->DisplaySize.x / 3), static_cast<float>(this->io->DisplaySize.y / 3)), ImGuiCond_Always);
		if (ImGui::Begin(ICON_FA_IMAGE " Histogram", &this->histogramPopUp, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (this->workStation.getImageStrP()->histogramR != NULL &&
				this->workStation.getImageStrP()->histogramG != NULL &&
				this->workStation.getImageStrP()->histogramB != NULL) {
				//The graph has to be this->toolsPanelPixels wide
				int maxValue = 0;
				if (this->workStation.getImageStrP()->histogramRSize > this->workStation.getImageStrP()->histogramGSize)
				{
					if (this->workStation.getImageStrP()->histogramRSize > this->workStation.getImageStrP()->histogramBSize)
					{
						maxValue = this->workStation.getImageStrP()->histogramRSize;
					}
					else {
						if (this->workStation.getImageStrP()->histogramGSize > this->workStation.getImageStrP()->histogramBSize)
						{
							maxValue = this->workStation.getImageStrP()->histogramGSize;
						}
						else
						{
							maxValue = this->workStation.getImageStrP()->histogramBSize;
						}
					}
				}

				//Set the limits of the graph
				ImPlot::SetNextAxesLimits(0, 256 + 1, 0, maxValue, 0);
			
				if (ImPlot::BeginPlot("Histogram",ImVec2(-1,-1), ImPlotAxisFlags_AutoFit)) {
					ImPlot::SetupAxes("Value", "Quantity", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
					//Plot the Histogram
					ImPlot::SetNextFillStyle(ImVec4(1, 0, 0, 1));
					ImPlot::SetNextLineStyle(ImVec4(1, 0, 0, 1));
					ImPlot::PlotBars("Red", this->workStation.getImageStrP()->histogramR, 256, 1.0f, 0.0f);
					ImPlot::SetNextFillStyle(ImVec4(0, 1, 0, 1));
					ImPlot::SetNextLineStyle(ImVec4(0, 1, 0, 1));
					ImPlot::PlotBars("Green", this->workStation.getImageStrP()->histogramG, 256, 1.0f, 0.0f);
					ImPlot::SetNextFillStyle(ImVec4(0, 0, 1, 1));
					ImPlot::SetNextLineStyle(ImVec4(0, 0, 1, 1));
					ImPlot::PlotBars("Blue", this->workStation.getImageStrP()->histogramB, 256, 1.0f, 0.0f);
				}
				ImPlot::EndPlot();

			}
			ImGui::End();
		}
	}
}
