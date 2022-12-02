#include "../ImageWork.h"

void ImageWork::toolColorPicker(int _MouseImagePositionX, int _MouseImagePositionY) {
	//Check if the mouse is inside the image
	if (_MouseImagePositionX >= 0 && _MouseImagePositionX < CurrentImage.width && _MouseImagePositionY >= 0 && _MouseImagePositionY < CurrentImage.height) {
		//Get the color of the pixel
		this->toolColor = this->getPixel(_MouseImagePositionX, _MouseImagePositionY);
	}
}