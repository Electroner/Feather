#include "../feathergui.h"

// Row layout constants
static const float ROW_H = 58.0f;
static const float THUMB = 46.0f;
static const float EYE_W = 22.0f;
static const float GAP   =  4.0f;

void FeatherGUI::BuildLayers() {
	float temp_pct = ((float)(static_cast<float>(this->propertiesPanelPixels) / static_cast<float>(this->windowWidth)));
	ImGui::SetNextWindowPos(ImVec2((1.0F - temp_pct) * this->io->DisplaySize.x, (this->io->DisplaySize.y + this->MenuSizePixels) / 2), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(temp_pct * this->io->DisplaySize.x, (this->io->DisplaySize.y - this->MenuSizePixels) / 2 - this->infoPanelPixels), ImGuiCond_Always);

	ImGui::Begin("Layers Menu", NULL,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::BeginTabBar("Layers", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Layers")) {

			// ---- Action buttons (2×2, icon + label) ----
			float bw2 = std::floor((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f);

			// Row 1
			if (ImGui::Button(ICON_FA_PLUS " New Layer", ImVec2(bw2, 0)) &&
			    this->workStation.ImagesSize() > 0) {
				this->workStation.pushUndoState();
				this->workStation.addNewLayer("Layer " + std::to_string(this->workStation.ImagesSize() + 1));
				this->UpdateImage();
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_COPY " Duplicate", ImVec2(bw2, 0)) &&
			    this->workStation.ImagesSize() > 0) {
				this->workStation.pushUndoState();
				this->workStation.duplicateLayer(this->workStation.getActiveLayerIndex());
				this->UpdateImage();
			}

			// Row 2
			if (ImGui::Button(ICON_FA_TRASH " Delete", ImVec2(bw2, 0)) &&
			    this->workStation.ImagesSize() > 1) {
				this->workStation.pushUndoState();
				this->workStation.removeLayer(this->workStation.getActiveLayerIndex());
				this->UpdateImage();
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_COMPRESS " Merge Down", ImVec2(bw2, 0))) {
				int idx = this->workStation.getActiveLayerIndex();
				if (idx < this->workStation.ImagesSize() - 1) {
					this->workStation.pushUndoState();
					this->workStation.mergeLayerDown(idx);
					this->UpdateImage();
				}
			}

			ImGui::Separator();

			// ---- Layer list ----
			ImDrawList* dl = ImGui::GetWindowDrawList();
			float panelW   = ImGui::GetContentRegionAvail().x;
			float txtX_off = GAP + EYE_W + GAP + THUMB + GAP;
			float txtW     = panelW - txtX_off - GAP;

			// Drag-to-reorder state (persists across frames)
			static int s_dragIdx = -1;
			float listTopY = ImGui::GetCursorScreenPos().y;

			for (int i = 0; i < this->workStation.ImagesSize(); i++) {
				ImageStr* layer = this->workStation.getImage(i);
				bool isActive   = (i == this->workStation.getActiveLayerIndex());

				ImGui::PushID(i);

				ImVec2 p0 = ImGui::GetCursorScreenPos();

				// Drag-in-progress indicator
				if (s_dragIdx >= 0 && s_dragIdx != i) {
					float mouseY = ImGui::GetMousePos().y;
					int hover = (int)((mouseY - listTopY) / (ROW_H + 1.0f));
					if (hover < 0) hover = 0;
					if (hover >= this->workStation.ImagesSize()) hover = this->workStation.ImagesSize() - 1;
					if (hover == i) {
						dl->AddRect(p0, ImVec2(p0.x + panelW, p0.y + ROW_H), IM_COL32(90, 150, 255, 200), 2.0f, 0, 2.0f);
					}
				}

				// Active layer highlight
				if (isActive)
					dl->AddRectFilled(p0, ImVec2(p0.x + panelW, p0.y + ROW_H), IM_COL32(55, 80, 160, 110));

				// Dragging source highlight
				if (s_dragIdx == i)
					dl->AddRectFilled(p0, ImVec2(p0.x + panelW, p0.y + ROW_H), IM_COL32(90, 150, 255, 60));

				// ---- Eye checkbox ----
				ImGui::SetCursorScreenPos(ImVec2(p0.x + GAP, p0.y + (ROW_H - 16.0f) * 0.5f));
				if (ImGui::Checkbox("##v", &layer->visible))
					this->UpdateImage();
				if (ImGui::IsItemHovered()) ImGui::SetTooltip(layer->visible ? "Hide" : "Show");

				// ---- Thumbnail ----
				float thX = p0.x + GAP + EYE_W + GAP;
				float thY = p0.y + (ROW_H - THUMB) * 0.5f;
				ImGui::SetCursorScreenPos(ImVec2(thX, thY));
				if (layer->texture != 0)
					ImGui::Image((ImTextureID)(intptr_t)layer->texture, ImVec2(THUMB, THUMB));
				else
					ImGui::Dummy(ImVec2(THUMB, THUMB));

				// ---- Name selectable (click = select, drag = reorder) ----
				float txtX = p0.x + txtX_off;
				ImGui::SetCursorScreenPos(ImVec2(txtX, p0.y + GAP));

				// Type icon prefix
				const char* typeIcon = layer->isTextLayer ? ICON_FA_FONT " " : ICON_FA_IMAGE " ";
				std::string lbl = std::string(typeIcon) + layer->name;
				if (layer->modified) lbl += "*";
				if (!layer->visible) lbl = "(" + lbl + ")";

				ImGui::PushStyleColor(ImGuiCol_Header,        isActive ? ImVec4(.3f,.45f,.9f,.4f) : ImVec4(0,0,0,0));
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(.5f,.5f,.5f,.25f));
				ImGui::PushStyleColor(ImGuiCol_HeaderActive,  ImVec4(.3f,.45f,.9f,.6f));
				if (ImGui::Selectable(lbl.c_str(), isActive, ImGuiSelectableFlags_AllowOverlap, ImVec2(txtW, 22.0f)))
					this->workStation.setActiveLayer(i);

				// Detect drag start on this selectable
				if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0, 6.0f))
					s_dragIdx = i;

				ImGui::PopStyleColor(3);

				// ---- Opacity slider (direct pointer — fixes both-layers bug) ----
				ImGui::SetCursorScreenPos(ImVec2(txtX, p0.y + ROW_H - GAP - 18.0f));
				ImGui::SetNextItemWidth(txtW);
				if (ImGui::SliderFloat("##op", &layer->opacity, 0.0f, 1.0f, "%.2f"))
					this->UpdateImage();
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Opacity");

				// Advance cursor past row
				ImGui::SetCursorScreenPos(ImVec2(p0.x, p0.y + ROW_H + 1.0f));
				ImGui::Dummy(ImVec2(panelW, 0));

				ImGui::PopID();
				ImGui::Separator();
			}

			// ---- Handle drag completion ----
			if (s_dragIdx >= 0 && ImGui::IsMouseReleased(0)) {
				float mouseY = ImGui::GetMousePos().y;
				int target = (int)((mouseY - listTopY) / (ROW_H + 1.0f));
				if (target < 0) target = 0;
				if (target >= this->workStation.ImagesSize()) target = this->workStation.ImagesSize() - 1;
				if (target != s_dragIdx) {
					this->workStation.pushUndoState();
					this->workStation.moveLayer(s_dragIdx, target);
					this->workStation.combineLayers();
					this->UpdateImage();
				}
				s_dragIdx = -1;
			}
			// Cancel drag if mouse leaves window
			if (s_dragIdx >= 0 && !ImGui::IsMouseDown(0))
				s_dragIdx = -1;

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::PopStyleColor(); // Pops WindowBg pushed in Properties.cpp
}
