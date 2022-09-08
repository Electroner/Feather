#include "ImageWork.h"

ImageWork::ImageWork() {}

ImageWork::~ImageWork() {}

void ImageWork::init(std::vector<ImageStr>** _Images, ImageStr** _CurrentImage) {
	*_Images = &this->Images;
	*_CurrentImage = &this->CurrentImage;
}

bool ImageWork::useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY) {
	
	switch (_tool)
	{
		case 0:
			toolPencil(_MouseImagePositionX, _MouseImagePositionY);
			break;
		case 1:
			toolBrush(_MouseImagePositionX, _MouseImagePositionY);
			break;
		case 2:
			toolEraser(_MouseImagePositionX, _MouseImagePositionY);
			break;
		
		default:
			break;
	}
	
	return true;
}

void ImageWork::toolPencil(int _MouseImagePositionX, int _MouseImagePositionY) {	
	int index = ((_MouseImagePositionY - 1) * this->CurrentImage.width * this->CurrentImage.channels) + ((_MouseImagePositionX - 1) * this->CurrentImage.channels);
	//Paint the pixel black
	CurrentImage.data[index] = 0;
	CurrentImage.data[index + 1] = 0;
	CurrentImage.data[index + 2] = 0;
	if (CurrentImage.channels == 4) {
		
		CurrentImage.data[index + 3] = 255;
	}
}

void ImageWork::toolBrush(int _MouseImagePositionX, int _MouseImagePositionY) {
	
}

void ImageWork::toolEraser(int _MouseImagePositionX, int _MouseImagePositionY){
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

void ImageWork::selectFrontImage() {
	//Set the current Image
	this->CurrentImage = this->Images.front();
}

void ImageWork::selectImage(int _index) {
	//Set the current Image
	this->CurrentImage = Images.at(_index);
}

void ImageWork::swapImage(int _indexa, int _indexb) {
	std::swap(this->Images.at(_indexa), this->Images.at(_indexb));
}