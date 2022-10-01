#include "../feathergui.h"

void FeatherGUI::BuildConsoleDebugMenu() {
	//DEBUG CONSOLE WINDOW
	//Set max size of the window to 1/4 of the screen
	ImGui::SetNextWindowSize(ImVec2(this->io->DisplaySize.x / 4, this->io->DisplaySize.y / 2), ImGuiCond_Always);
	ImGui::Begin("Debug Console", &this->debugConsole, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	//FPS
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//Print the mouse
	ImGui::Text("Mouse Position: (%d, %d)", this->MouseImagePositionX, this->MouseImagePositionY);
	//Slider int
	ImGui::Text("Movement X: %d", this->imageShiftX);
	ImGui::Text("Movement Y: %d", this->imageShiftY);
	//Print the zoom
	ImGui::Text("Zoom: %.2f", this->zoom);
	//Print the maximun zoom
	ImGui::Text("Max Zoom: %.2f", this->maxZoom);
	//Print the minimun zoom
	ImGui::Text("Min Zoom: %.2f", this->minZoom);


	ImGui::Separator();

	//if (ss.str().c_str() != NULL) {
	//	//Get time and add a time mark using localtime_s
	//	time_t rawtime;
	//	struct tm now;
	//	char timeMark[80];
	//	time(&rawtime);
	//	localtime_s(&now, &rawtime);
	//	strftime(timeMark, 80, "%H:%M:%S", &now);	
	//}


	ImGui::Text("%s", ss.str().c_str());
	ImGui::End();
}
