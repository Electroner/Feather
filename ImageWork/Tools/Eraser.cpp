#include "../ImageWork.h"

void ImageWork::toolEraser(int _MouseImagePositionX, int _MouseImagePositionY, int _radius) {
	//paint the pixel white
	int index = ((_MouseImagePositionY - 1) * this->CurrentImage.width * this->CurrentImage.channels) + ((_MouseImagePositionX - 1) * this->CurrentImage.channels);
	//Paint the pixel black
	this->CurrentImage.data[index] = 255;
	this->CurrentImage.data[index + 1] = 255;
	this->CurrentImage.data[index + 2] = 255;
	if (this->CurrentImage.channels == 4) {

		this->CurrentImage.data[index + 3] = 255;
	}
}