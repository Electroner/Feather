#include "ImageWork.h"

void ImageWork::init(std::vector<ImageStr>** _Images, ImageStr** _CurrentImage) {
	*_Images = &this->Images;
	*_CurrentImage = &this->CurrentImage;

	this->interpolationMin = std::pair<int,int>(this->CurrentImage.width, this->CurrentImage.height);
	this->interpolationMax = std::pair<int, int>(0, 0);
	
	temp = 0;
}

std::pair<int, int> ImageWork::getInterpolationMin() {
	return this->interpolationMin;
}

std::pair<int, int> ImageWork::getInterpolationMax() {
	return this->interpolationMax;
}

void ImageWork::clearMousePoints() {
	this->mousePoints.clear();
}

void ImageWork::clearMousePairs() {
	this->interpolationMin = std::pair<int, int>(this->CurrentImage.width, this->CurrentImage.height);
	this->interpolationMax = std::pair<int, int>(0, 0);
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
	//Add point to mousePoints
	mousePoints.push_back(std::make_pair(_MouseImagePositionX, _MouseImagePositionY));
	
	//Update interpolation min and max
	if (this->interpolationMin.first > _MouseImagePositionX) {
		this->interpolationMin.first = _MouseImagePositionX;
	}
	if (this->interpolationMin.second > _MouseImagePositionY) {
		this->interpolationMin.second = _MouseImagePositionY;
	}
	if (this->interpolationMax.first < _MouseImagePositionX) {
		this->interpolationMax.first = _MouseImagePositionX;
	}
	if (this->interpolationMax.second < _MouseImagePositionY) {
		this->interpolationMax.second = _MouseImagePositionY;
	}

	//If there is only one point, set the pixel
	if (!mousePoints.empty()) {
		int index = (_MouseImagePositionY * this->CurrentImage.width * this->CurrentImage.channels) + (_MouseImagePositionX * this->CurrentImage.channels);
		CurrentImage.data[index] = static_cast<char>(this->toolcolor.r * 255);
		CurrentImage.data[index + 1] = static_cast<char>(this->toolcolor.g * 255);
		CurrentImage.data[index + 2] = static_cast<char>(this->toolcolor.b * 255);
		if (CurrentImage.channels == 4) {

			CurrentImage.data[index + 3] = 255;
		}
	}

	//If there are more than 2 points, interpolate
	if (mousePoints.size() > 1) {
		//Interpolate
		int x1 = mousePoints[mousePoints.size() - 2].first;
		int y1 = mousePoints[mousePoints.size() - 2].second;
		int x2 = mousePoints[mousePoints.size() - 1].first;
		int y2 = mousePoints[mousePoints.size() - 1].second;
		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
		int sx = x1 < x2 ? 1 : -1;
		int sy = y1 < y2 ? 1 : -1;
		int err = dx - dy;
		int e2;
		while (true) {
			//Set pixel
			int index = (y1 * CurrentImage.width + x1) * CurrentImage.channels;
			CurrentImage.data[index] = static_cast<char>(toolcolor.r * 255);
			CurrentImage.data[index + 1] = static_cast<char>(toolcolor.g * 255);
			CurrentImage.data[index + 2] = static_cast<char>(toolcolor.b * 255);
			if (this->CurrentImage.channels == 4) {
				CurrentImage.data[index + 3] = 255;
			}
			if (x1 == x2 && y1 == y2) break;
			e2 = 2 * err;
			if (e2 > -dy) { err -= dy; x1 += sx; }
			if (e2 < dx) { err += dx; y1 += sy; }
		}
	}
	
	//If the size is 10 or more clear the vector and let the last two points be the first points for optimization
	if (mousePoints.size() >= 10) {
		std::pair<int, int> lastPoint = mousePoints[mousePoints.size() - 1];
		std::pair<int, int> secondLastPoint = mousePoints[mousePoints.size() - 2];
		mousePoints.clear();
		mousePoints.push_back(secondLastPoint);
		mousePoints.push_back(lastPoint);
		
		//Calculate the new interpolation min and max values with the last two points
		if (lastPoint.first < secondLastPoint.first) {
			this->interpolationMin.first = lastPoint.first;
			this->interpolationMax.first = secondLastPoint.first;
		}
		else {
			this->interpolationMin.first = secondLastPoint.first;
			this->interpolationMax.first = lastPoint.first;
		}
		if (lastPoint.second < secondLastPoint.second) {
			this->interpolationMin.second = lastPoint.second;
			this->interpolationMax.second = secondLastPoint.second;
		}
		else {
			this->interpolationMin.second = secondLastPoint.second;
			this->interpolationMax.second = lastPoint.second;
		}
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

void ImageWork::setToolColor(RGB _color) {
	this->toolcolor = _color;
}

RGB ImageWork::getToolColor() {
	return this->toolcolor;
}