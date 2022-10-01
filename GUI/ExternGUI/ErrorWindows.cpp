#include "../feathergui.h"

void FeatherGUI::ErrorWindowCreateImage() {
	if (this->errorWindowCreateImage) {
		ImGui::OpenPopup("Error Creating Image");
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		//Show a BeginPopupModal dialog with error message "The size of the Image must be greater than 0. Also it need to have a name!\n\n"				
		if (ImGui::BeginPopupModal("Error Creating Image", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::SetItemDefaultFocus();
			ImGui::Text("Some of the following things were wrong: \n\n-> The size of the Image must be greater than 0.\n-> It need to have a name.\n\n");
			//ImGui::Separator();

			if (ImGui::Button("OK")) {
				this->errorWindowCreateImage = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}