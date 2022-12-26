#include "../feathergui.h"

void FeatherGUI::BuildConsoleDebugMenu() {
	//DEBUG CONSOLE WINDOW
	//Set max size of the window to 1/4 of the screen
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(this->io->DisplaySize.x / 4), static_cast<float>(this->io->DisplaySize.y / 1.5)), ImGuiCond_Always);
	//ImGui::Begin("Debug Console", &this->debugConsole, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Begin("Debug Console", &this->debugConsole, ImGuiWindowFlags_NoCollapse);
	//IMGUI TEXT
	ImGui::Text("SYSTEM STATUS");
	ImGui::Separator();
	//Windows VARS
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	//Show avaliable ullTotalPhys memory / 1024
	ImGui::Text("Avaliable Memory: %d MB", status.ullTotalPhys / 1024);
	//Show used ullAvailPhys memory / 1024
	ImGui::Text("Used Memory: %d MB", (status.ullTotalPhys - status.ullAvailPhys) / 1024);
	//Show Difference (Memory free)
	ImGui::Text("Memory Free: %d MB", status.ullAvailPhys / 1024);
	//Show dwMemoryLoad
	ImGui::Text("Memory Load: %d%%", status.dwMemoryLoad);
	//Show Free memory %
	ImGui::Text("Free Memory: %d%%", (status.ullAvailPhys * 100) / status.ullTotalPhys);

	
	//IMGUI TEXT
	ImGui::Text("\nFEATHER STATUS");
	ImGui::Separator();
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
	
	ImGui::Text("\nCONSOLE");
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

	//Create a box of text with the log
	//Begin
	ImGui::BeginChild("Scrolling");	
	//Print the log
	ImGui::Text("%s", ss.str().c_str());
	//AutoScrool to the end of this text
	ImGui::SetScrollHereY(1.0f);
	//End the child
	ImGui::EndChild();
	
	ImGui::End();
}
