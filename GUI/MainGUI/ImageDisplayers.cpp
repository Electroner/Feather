#include "../feathergui.h"

void FeatherGUI::BuildImageDisplayer() {
	//IMAGE WINDOW
	ImGui::SetNextWindowPos(ImVec2((float)(static_cast<float>(this->toolsPanelPixels) / static_cast<float>(this->windowWidth)) * (float)io->DisplaySize.x, (float)this->MenuSizePixels), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x, io->DisplaySize.y - this->MenuSizePixels - this->infoPanelPixels), ImGuiCond_Always);
	ImGui::Begin("Image Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);

	//Check if this windows is in the top and nothing else is over it
	if (ImGui::IsItemActive() && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows)) {
		this->MouseOverImageWindow = true;
	}
	else
	{
		this->MouseOverImageWindow = false;
	}

	//Move the image
	ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
	//Draw Image
	if (!this->Images->empty()) {
		if (this->CurrentImage->data != NULL) {
			ImGui::Image((void*)(intptr_t)this->CurrentImage->texture, ImVec2(this->CurrentImage->width * this->zoom, this->CurrentImage->height * this->zoom));
		}
	}

#define DEBUG
#ifdef DEBUG

	//Draw the update area interpolated
	ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
	ImGui::GetWindowDrawList()->AddRect(
		ImVec2(this->workStation.getInterpolationMin().first * this->zoom + this->imageShiftX + this->toolsPanelPixels,
			this->workStation.getInterpolationMin().second * this->zoom + this->imageShiftY + this->MenuSizePixels),
		ImVec2(this->workStation.getInterpolationMax().first * this->zoom + this->imageShiftX + this->toolsPanelPixels,
			this->workStation.getInterpolationMax().second * this->zoom + this->imageShiftY + this->MenuSizePixels),
		IM_COL32(128, 128, 128, 255), 0.0F, 0, 2.0F);
	
#endif // DEBUG

	//Draw Image Border, take into account the zoom and the shift
	if (!this->Images->empty()) {
		int locar = ((static_cast<int>(this->BackGroundRGB.r * 255 + 128) % 255));
		int locag = ((static_cast<int>(this->BackGroundRGB.g * 255 + 128) % 255));
		int locab = ((static_cast<int>(this->BackGroundRGB.b * 255 + 128) % 255));
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(static_cast<float>(this->imageShiftX + this->toolsPanelPixels),
				static_cast<float>(this->imageShiftY + this->MenuSizePixels)),
			ImVec2(this->CurrentImage->width * this->zoom + this->imageShiftX + this->toolsPanelPixels,
				this->CurrentImage->height * this->zoom + this->imageShiftY + this->MenuSizePixels),
			IM_COL32(locar, locag, locab, 255), 0.0F, 0, 2.0F);
	}

	//if selection is enable draw the selection
	if (this->workStation.getSelectionEnabled()) {
		ImGui::SetCursorPos(ImVec2((float)this->imageShiftX, (float)this->imageShiftY));
		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMin().first * this->zoom, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMin().second * this->zoom),
			ImVec2(this->imageShiftX + this->toolsPanelPixels + this->workStation.getSelectionMax().first * this->zoom, this->imageShiftY + this->MenuSizePixels + this->workStation.getSelectionMax().second * this->zoom),
			IM_COL32(128, 128, 128, 255), 0.0F, 0, 2.0F);
	}
	

	ImGui::End();
}

void FeatherGUI::calculateZoom() {
	//Whit the size of the image and the size of the screen, calculate the zoom to fit the image in the screen letting 32 pixels of space on the closest side
	//If the image is taller than widder then the image will be zoomed in vertically and horizontally
	//If the image is wider than taller then the image will be zoomed in horizontally and vertically

	//Window Sizes
	int size_x = static_cast<int>((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x);
	int size_y = static_cast<int>(io->DisplaySize.y - this->MenuSizePixels - this->infoPanelPixels);

	//Image Sizes
	int image_x = this->CurrentImage->width;
	int image_y = this->CurrentImage->height;

	//Calculate the zoom to fit the image in the secreen
	if (image_x / size_x > image_y / size_y) {
		this->zoom = (float)size_x / ((float)image_x + 32.0F);
	}
	else {
		this->zoom = (float)size_y / ((float)image_y + 32.0F);
	}

	//TODO Improve this

	//Set the minimun to fit 100 pixels in a pixel
	this->minZoom = 100.0F / ((float)image_x);

	float diff = this->zoom - this->minZoom;

	//Set the max zoom with the diff multiplied by the factor of adjust
	this->maxZoom = this->zoom + diff * 32;

	//Set the increment in the 1% of the diference between min and max
	this->zoomIncrement = (this->maxZoom - this->minZoom) / 100.0F;

	std::cout << "Adjusting zoom of " << this->CurrentImage->name << std::endl;
	std::cout << "Adjusted Zoom to: " << this->zoom << std::endl;
	std::cout << "Adjusted Max Zoom: " << this->maxZoom << std::endl;
	std::cout << "Adjusted Min Zoom: " << this->minZoom << std::endl;
	std::cout << "Adjusted Zoom Increment: " << this->zoomIncrement << std::endl;
}

void FeatherGUI::centerImage() {
	calculateZoom();
	//Center X is (1-(x+z))/2+x, where x is toolsPanelPixels and z is propertiesPanelPixels, substract the size of the image
	this->imageShiftX = static_cast<int>((1.0F - (float)(static_cast<float>(this->toolsPanelPixels + this->propertiesPanelPixels) / static_cast<float>(this->windowWidth))) * io->DisplaySize.x / 2.0F - (this->CurrentImage->width * this->zoom / 2.0F));
	//Center Y is the size of the screen minus the size of the image divided by 2
	this->imageShiftY = static_cast<int>((io->DisplaySize.y - this->infoPanelPixels - this->MenuSizePixels) / 2.0F - (this->CurrentImage->height * this->zoom / 2.0F));

	std::cout << "Adjusted Image Shift X: " << this->imageShiftX << std::endl;
	std::cout << "Adjusted Image Shift Y: " << this->imageShiftY << std::endl;
}

void FeatherGUI::UpdateImage() {
	//Select current texture and bind it
	glBindTexture(GL_TEXTURE_2D, this->CurrentImage->texture);

	////Method 1: Update the entire image
	//if (this->CurrentImage->channels == 4) {
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->CurrentImage->width, this->CurrentImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->CurrentImage->data);
	//}
	//else 
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->CurrentImage->width, this->CurrentImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->CurrentImage->data);
	//}

	//Method 2: Update the rectangle of the image that has been modified during the interpolation
	//Calculate the width and height of the rectangle
	int width = workStation.getInterpolationMax().first - workStation.getInterpolationMin().first + 1;
	int height = workStation.getInterpolationMax().second - workStation.getInterpolationMin().second + 1;

	//Create and reserve a pixel array to store the rectangle of the image
	unsigned char* pixel = new unsigned char[width * height * this->CurrentImage->channels];
	//Store the image data in the pixel array
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < this->CurrentImage->channels; k++) {
				pixel[(i * width + j) * this->CurrentImage->channels + k] = 
					this->CurrentImage->data[((i + workStation.getInterpolationMin().second) * 
						this->CurrentImage->width + j + workStation.getInterpolationMin().first) * 
							this->CurrentImage->channels + k];
			}
		}
	} 

	if (this->CurrentImage->channels == 4)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, workStation.getInterpolationMin().first, workStation.getInterpolationMin().second, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	}
	else
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, workStation.getInterpolationMin().first, workStation.getInterpolationMin().second, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	}
	//Delete the pixel array
	delete[] pixel;

	////Method 3: Update the pixel that has been modified during the interpolation with glMapBuffer
	//int width = workStation.getInterpolationMax().first - workStation.getInterpolationMin().first + 1;
	//int height = workStation.getInterpolationMax().second - workStation.getInterpolationMin().second + 1;
	//
	//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *this->CurrentImage->data);
	//unsigned char* pixel = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	//if (pixel) {
	//	for (int i = 0; i < height; i++) {
	//		for (int j = 0; j < width; j++) {
	//			for (int k = 0; k < this->CurrentImage->channels; k++) {
	//				pixel[(i * width + j) * this->CurrentImage->channels + k] =
	//					this->CurrentImage->data[((i + workStation.getInterpolationMin().second) *
	//						this->CurrentImage->width + j + workStation.getInterpolationMin().first) *
	//					this->CurrentImage->channels + k];
	//			}
	//		}
	//	}
	//	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	//}
	//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	//
	////Unbind the texture
	//glBindTexture(GL_TEXTURE_2D, 0);
	
}