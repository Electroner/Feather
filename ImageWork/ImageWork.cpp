#include "./ImageWork.h"

void ImageWork::init(std::vector<ImageStr>** _Images, ImageStr** _CurrentImage) {
	*_Images = &this->Images;
	*_CurrentImage = &this->CurrentImage;

	this->interpolationMin = std::pair<int, int>(this->CurrentImage.width, this->CurrentImage.height);
	this->interpolationMax = std::pair<int, int>(0, 0);

	this->selectionMin = std::pair<int, int>(0, 0);
	this->selectionMax = std::pair<int, int>(0, 0);

	this->toolradius = 1;
	this->selectionEnable = false;

	//First point of the selection to -1
	this->firstPoint = std::pair<int, int>(-1, -1);
}

std::pair<int, int> ImageWork::getInterpolationMin() {
	return this->interpolationMin;
}

std::pair<int, int> ImageWork::getInterpolationMax() {
	return this->interpolationMax;
}

std::pair<int, int> ImageWork::getSelectionMin() {
	return this->selectionMin;
}

std::pair<int, int> ImageWork::getSelectionMax() {
	return this->selectionMax;
}

void ImageWork::clearMousePoints() {
	this->mousePoints.clear();
}

void ImageWork::clearMousePairs() {
	this->interpolationMin = std::pair<int, int>(this->CurrentImage.width, this->CurrentImage.height);
	this->interpolationMax = std::pair<int, int>(0, 0);
}

void ImageWork::clearFirstPointSelection() {
	this->firstPoint = std::pair<int, int>(-1, -1);
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

bool ImageWork::useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY) {

	switch (_tool)
	{
	case 0:
		toolPencil(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case 1:
		toolBrush(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case 2:
		toolEraser(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case 3:
		toolSelection(_MouseImagePositionX, _MouseImagePositionY);
		break;
		
	default:
		this->setSelectionEnable(false);
		break;
	}

	return true;
}

void ImageWork::setToolColor(RGB _color) {
	this->toolcolor = _color;
}

void ImageWork::setToolRadius(int _radius) {
	this->toolradius = _radius;
}

void ImageWork::setSelectionEnable(bool _enable) {
	this->selectionEnable = _enable;
}

RGB ImageWork::getToolColor() {
	return this->toolcolor;
}

int ImageWork::getToolRadius() {
	return this->toolradius;
}

bool ImageWork::getSelectionEnabled() {
	return this->selectionEnable;
}

void ImageWork::setPixel(int _x, int _y, RGB _color) {
	//Check if the pixel is in the image
	if (_x >= 0 && _x < this->CurrentImage.width && _y >= 0 && _y < this->CurrentImage.height) {
		int index = (_y * this->CurrentImage.width + _x) * this->CurrentImage.channels;
		CurrentImage.data[index] = static_cast<char>(toolcolor.r * 255);
		CurrentImage.data[index + 1] = static_cast<char>(toolcolor.g * 255);
		CurrentImage.data[index + 2] = static_cast<char>(toolcolor.b * 255);
		if (CurrentImage.channels == 4) {
			CurrentImage.data[_x + _y * CurrentImage.width + 3] = static_cast<char>(_color.delta);
		}
	}
}