#include "../feathergui.h"

void FeatherGUI::ErrorWindow() {
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	
	switch (this->errorWindowCode)
	{
	case 0:
		//No error
		break;
	case 1:
		//No memory to create a new image
		ImGui::OpenPopup("Error Creating Image");
		// Always center this window when appearing
		
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		//Show a BeginPopupModal dialog with error message "The size of the Image must be greater than 0. Also it need to have a name!\n\n"				
		if (ImGui::BeginPopupModal("Error Creating Image", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::SetItemDefaultFocus();
			ImGui::Text("There is not enough memory to create the image!\n\n");
			//ImGui::Separator();

			if (ImGui::Button("OK")) {
				this->errorWindowCode = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		break;
	case 2:
		//No Name or dimensions for the new image error
		ImGui::OpenPopup("Error Creating Image");
		// Always center this window when appearing
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		//Show a BeginPopupModal dialog with error message "The size of the Image must be greater than 0. Also it need to have a name!\n\n"				
		if (ImGui::BeginPopupModal("Error Creating Image", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::SetItemDefaultFocus();
			ImGui::Text("Some of the following things were wrong: \n\n-> The size of the Image must be greater than 0.\n-> It need to have a name.\n\n");
			//ImGui::Separator();

			if (ImGui::Button("OK")) {
				this->errorWindowCode = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		break;

	default:
		break;
	}
}