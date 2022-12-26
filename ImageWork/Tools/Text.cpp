#include "../ImageWork.h"

void ImageWork::toolText(int _MouseImagePositionX, int _MouseImagePositionY) {
	this->textEnable = true;
	this->text = "Hello World!";
	this->textPosition.first = _MouseImagePositionX;
	this->textPosition.second = _MouseImagePositionY;

}