#include "../ImageWork.h"

void ImageWork::toolEraser(int _MouseImagePositionX, int _MouseImagePositionY, int _radius) {
	//Paint the image with the secondary color (background color) in the area of the eraser
	for (int y = _MouseImagePositionY - _radius; y < _MouseImagePositionY + _radius; y++) {
		for (int x = _MouseImagePositionX - _radius; x < _MouseImagePositionX + _radius; x++) {
			//Check if the pixel is in the image
			if (x >= 0 && x < this->CurrentImage.width && y >= 0 && y < this->CurrentImage.height) {
				int index = (y * this->CurrentImage.width + x) * this->CurrentImage.channels;
				this->CurrentImage.data[index] = this->secondaryColor.r;
				this->CurrentImage.data[index + 1] = this->secondaryColor.g;
				this->CurrentImage.data[index + 2] = this->secondaryColor.b;
				if (this->CurrentImage.channels == 4) {
					this->CurrentImage.data[index + 3] = this->secondaryColor.delta;
				}
			}
		}
	}
}