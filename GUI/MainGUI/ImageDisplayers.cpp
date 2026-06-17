#include "../feathergui.h"

void FeatherGUI::BuildImageDisplayer() {
	//IMAGE WINDOW
	ImGui::SetNextWindowPos(ImVec2((float)(static_cast<float>(this->toolsPanelPixels) / static_cast<float>(this->windowWidth)) * (float)io->DisplaySize.x, (float)this->MenuSizePixels), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x, io->DisplaySize.y - this->MenuSizePixels - this->infoPanelPixels), ImGuiCond_Always);
	ImGui::Begin("Image Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);

	//Check if this windows is in the top and nothing else is over it
	if (/*ImGui::IsItemActive() &&*/ ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows)) {
		this->MouseOverImageWindow = true;
	}
	else
	{
		this->MouseOverImageWindow = false;
	}

	if (this->workStation.ImagesSize() != 0) {
		int  cw  = this->workStation.getCanvasWidth();
		int  ch  = this->workStation.getCanvasHeight();
		float sw = cw * this->zoom;
		float sh = ch * this->zoom;
		float bx = (float)(this->imageShiftX + this->toolsPanelPixels);
		float by = (float)(this->imageShiftY + this->MenuSizePixels);
		ImDrawList* dl = ImGui::GetWindowDrawList();

		// ---- Checkerboard for transparent areas ----
		static GLuint checkTex = 0;
		if (checkTex == 0) {
			// 2x2 RGBA checkerboard tile (light / dark)
			unsigned char pix[16] = {
				200,200,200,255,  140,140,140,255,
				140,140,140,255,  200,200,200,255
			};
			glGenTextures(1, &checkTex);
			glBindTexture(GL_TEXTURE_2D, checkTex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);
		}
		// Each check = 8 screen pixels; texture is 2×2 = 2 checks wide → UV max = sw / (2*8)
		const float CHECK_PX = 8.0f;
		float uvW = sw / (2.0f * CHECK_PX);
		float uvH = sh / (2.0f * CHECK_PX);
		dl->AddImageQuad(
			(ImTextureID)(intptr_t)checkTex,
			ImVec2(bx, by), ImVec2(bx + sw, by), ImVec2(bx + sw, by + sh), ImVec2(bx, by + sh),
			ImVec2(0, 0), ImVec2(uvW, 0), ImVec2(uvW, uvH), ImVec2(0, uvH));

		// ---- Composited image ----
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImageStr* composite = this->workStation.getCompositeImage();
		if (composite->texture != 0) {
			ImGui::Image((void*)(intptr_t)composite->texture, ImVec2(sw, sh));
		}

#ifdef DEBUG
		dl->AddRect(
			ImVec2(this->workStation.getInterpolationMin().first  * this->zoom + bx,
			       this->workStation.getInterpolationMin().second * this->zoom + by),
			ImVec2(this->workStation.getInterpolationMax().first  * this->zoom + bx,
			       this->workStation.getInterpolationMax().second * this->zoom + by),
			IM_COL32(128, 128, 128, 255), 0.0f, 0, 2.0f);
#endif

		// ---- Canvas border ----
		int locar = ((static_cast<int>(this->BackGroundRGB.r * 255 + 128) % 255));
		int locag = ((static_cast<int>(this->BackGroundRGB.g * 255 + 128) % 255));
		int locab = ((static_cast<int>(this->BackGroundRGB.b * 255 + 128) % 255));
		dl->AddRect(ImVec2(bx, by), ImVec2(bx + sw, by + sh),
			IM_COL32(locar, locag, locab, 255), 0.0f, 0, 2.0f);
	}

	//We call the function that will draw the things of tools if corresponds
	toolDisplays();

	ImGui::End();
}

void FeatherGUI::calculateZoom() {
	int image_x = this->workStation.getCanvasWidth();
	int image_y = this->workStation.getCanvasHeight();
	if (image_x <= 0 || image_y <= 0) return;

	int size_x = static_cast<int>((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x);
	int size_y = static_cast<int>(io->DisplaySize.y - this->MenuSizePixels - this->infoPanelPixels);
	if (size_x <= 0 || size_y <= 0) return;

	if ((float)image_x / size_x > (float)image_y / size_y)
		this->zoom = (float)size_x / ((float)image_x + 32.0F);
	else
		this->zoom = (float)size_y / ((float)image_y + 32.0F);

	this->minZoom       = 100.0F / (float)image_x;
	float diff          = this->zoom - this->minZoom;
	this->maxZoom       = this->zoom + diff * 32;
	this->zoomIncrement = (this->maxZoom - this->minZoom) / 100.0F;
}

void FeatherGUI::centerImage() {
	calculateZoom();
	int cw = this->workStation.getCanvasWidth();
	int ch = this->workStation.getCanvasHeight();
	this->imageShiftX = static_cast<int>((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x / 2.0F - (cw * this->zoom / 2.0F));
	this->imageShiftY = static_cast<int>((io->DisplaySize.y - this->infoPanelPixels - this->MenuSizePixels) / 2.0F - (ch * this->zoom / 2.0F));

	//std::cout << "Adjusted Image Shift X: " << this->imageShiftX << std::endl;
	//std::cout << "Adjusted Image Shift Y: " << this->imageShiftY << std::endl;
}

void FeatherGUI::UpdateImage() {
	if (this->workStation.ImagesSize() == 0) return;

	ImageStr* active = this->workStation.getImageStrP();
	if (!active || !active->data || !active->texture) return;

	int cw = this->workStation.getCanvasWidth();
	int ch = this->workStation.getCanvasHeight();

	// Check if tools reported a valid dirty rect this frame
	auto dMin = this->workStation.getInterpolationMin();
	auto dMax = this->workStation.getInterpolationMax();

	bool hasDirty = (dMin.first  <= dMax.first  &&
	                 dMin.second <= dMax.second  &&
	                 dMin.first  <  cw && dMin.second <  ch &&
	                 dMax.first  >= 0  && dMax.second >= 0);

	// We also need the composite buffer to already be initialized
	hasDirty = hasDirty && (this->workStation.getCompositeImage()->data != nullptr);

	if (hasDirty) {
		// Clamp region
		int x0 = dMin.first;
		int y0 = dMin.second;
		int x1 = dMax.first  + 1;
		int y1 = dMax.second + 1;
		if (x0 < 0)  x0 = 0;
		if (y0 < 0)  y0 = 0;
		if (x1 > cw) x1 = cw;
		if (y1 > ch) y1 = ch;
		int rw = x1 - x0, rh = y1 - y0;

		if (rw > 0 && rh > 0) {
			// Partial active-layer upload
			glBindTexture(GL_TEXTURE_2D, active->texture);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, cw);
			glTexSubImage2D(GL_TEXTURE_2D, 0, x0, y0, rw, rh,
			                GL_RGBA, GL_UNSIGNED_BYTE,
			                active->data + (y0 * cw + x0) * 4);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

			// Composite only the dirty region
			this->workStation.compositeRegion(x0, y0, rw, rh);
		}
	} else {
		// Full upload + full composite (e.g. after filter, layer op, load)
		GLenum fmt = (active->channels == 4) ? GL_RGBA : GL_RGB;
		glBindTexture(GL_TEXTURE_2D, active->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, fmt, active->width, active->height,
		             0, fmt, GL_UNSIGNED_BYTE, active->data);
		this->workStation.compositeAllLayers();
	}
}