#include "./ImageWork.h"

void ImageWork::init() {
	
	//set to false every loaded image in the vector
	for (int i = 0; i < this->Images.size(); i++) {
		Images.at(i).loaded = false;
	}

	this->CavasSize = std::make_pair(-1, -1);

	this->interpolationMin = std::pair<int, int>(this->CurrentImage.width, this->CurrentImage.height);
	this->interpolationMax = std::pair<int, int>(0, 0);

	this->selectionMin = std::pair<int, int>(0, 0);
	this->selectionMax = std::pair<int, int>(0, 0);

	this->toolradius = 5;
	this->selectionEnable = false;
	this->selectionDone = false;

	//Colors
	this->toolColor = RGB(0, 0, 0, 1);
	this->secondaryColor = RGB(1, 1, 1, 1);

	//First point of the selection to -1
	this->firstPoint = std::pair<int, int>(-1, -1);
}

void ImageWork::combineLayers() {
	//Set the current image to the first image in the vector
	this->CurrentImage = this->Images.front();
}

std::pair<int, int> ImageWork::getInterpolationMin() {
	return this->interpolationMin;
}

std::pair<int, int> ImageWork::getInterpolationMax() {
	return this->interpolationMax;
}

void ImageWork::setInterpolationMin(std::pair<int, int> _interpolationMin) {
	this->interpolationMin = _interpolationMin;
}

void ImageWork::setInterpolationMax(std::pair<int, int> _interpolationMax) {
	this->interpolationMax = _interpolationMax;
}

std::pair<int, int> ImageWork::getSelectionMin() {
	return this->selectionMin;
}

std::pair<int, int> ImageWork::getSelectionMax() {
	return this->selectionMax;
}

void ImageWork::setSelectionMin(std::pair<int, int> _selectionMin) {
	this->selectionMin = _selectionMin;
}

void ImageWork::setSelectionMax(std::pair<int, int> _selectionMax) {
	this->selectionMax = _selectionMax;
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

void ImageWork::clearSelection() {
	this->selectionMin = std::pair<int, int>(0, 0);
	this->selectionMax = std::pair<int, int>(0, 0);
}

void ImageWork::clearImages() {
	//free every data in the vector
	for (int i = 0; i < this->Images.size(); i++) {
		free(this->Images[i].data);
	}
	this->Images.clear();
}

void ImageWork::PushNewImage(ImageStr _Image) {
	this->Images.push_back(_Image);
}

int ImageWork::ImagesSize() {
	return this->Images.size();
}

ImageStr* ImageWork::getImage(int _index) {
	return &this->Images.at(_index);
}

void ImageWork::swapImages(int _indexa, int _indexb) {
	std::swap(this->Images.at(_indexa), this->Images.at(_indexb));
}

ImageStr* ImageWork::getImageStrP() {
	return &this->CurrentImage;
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
	this->toolColor = _color;
}

void ImageWork::setSecondaryColor(RGB _color) {
	this->secondaryColor = _color;
}

void ImageWork::setToolRadius(int _radius) {
	this->toolradius = _radius;
}

void ImageWork::setSelectionEnable(bool _enable) {
	this->selectionEnable = _enable;
}

void ImageWork::setSelectionDone(bool _done) {
	this->selectionDone = _done;
}

void ImageWork::selectionNormalize() {
	if (this->selectionMin.first > this->selectionMax.first) {
		std::swap(this->selectionMin.first, this->selectionMax.first);
	}
	if (this->selectionMin.second > this->selectionMax.second) {
		std::swap(this->selectionMin.second, this->selectionMax.second);
	}
}

RGB ImageWork::getToolColor() {
	return this->toolColor;
}

RGB ImageWork::getSecondaryColor() {
	return this->secondaryColor;
}

int ImageWork::getToolRadius() {
	return this->toolradius;
}

bool ImageWork::getSelectionEnabled() {
	return this->selectionEnable;
}

bool ImageWork::getSelectionDone() {
	return this->selectionDone;
}

void ImageWork::reCopyImage(ImageStr* _Image) {
	this->Images.front() = *_Image;
}

void ImageWork::resizeImage(ImageStr* _image, int _width, int _height) {
	//Check if the image is not null
	if (_image != nullptr) {
		//Check if the image is not empty
		if (_image->width > 0 && _image->height > 0) {
			//Check if the image is not the same size
			if (_image->width != _width || _image->height != _height) {
				//Resize the image
				GLubyte* newData = new GLubyte[_width * _height * 4];

				//Bilinear interpolation for resampling the new image
				for (int i = 0; i < _height; i++) {
					for (int j = 0; j < _width; j++) {
						float x = (float)j / (float)_width * (float)_image->width;
						float y = (float)i / (float)_height * (float)_image->height;

						int x1 = (int)floor(x);
						int x2 = (int)ceil(x);
						int y1 = (int)floor(y);
						int y2 = (int)ceil(y);

						float a = x - x1;
						float b = y - y1;

						//Get the 4 pixels
						RGB p1 = getPixel(_image, x1, y1);
						RGB p2 = getPixel(_image, x2, y1);
						RGB p3 = getPixel(_image, x1, y2);
						RGB p4 = getPixel(_image, x2, y2);

						//Interpolate the 4 pixels
						RGB p12;
						p12.r = (1 - a) * p1.r + a * p2.r;
						p12.g = (1 - a) * p1.g + a * p2.g;
						p12.b = (1 - a) * p1.b + a * p2.b;
						p12.delta = (1 - a) * p1.delta + a * p2.delta;

						RGB p34;
						p34.r = (1 - a) * p3.r + a * p4.r;
						p34.g = (1 - a) * p3.g + a * p4.g;
						p34.b = (1 - a) * p3.b + a * p4.b;
						p34.delta = (1 - a) * p3.delta + a * p4.delta;

						//Interpolate the 2 pixels
						RGB p1234;
						p1234.r = (1 - b) * p12.r + b * p34.r;
						p1234.g = (1 - b) * p12.g + b * p34.g;
						p1234.b = (1 - b) * p12.b + b * p34.b;
						p1234.delta = (1 - b) * p12.delta + b * p34.delta;

						//Set the new pixel
						newData[i * _width * 4 + j * 4 + 0] = (GLubyte)(p1234.r * 255);
						newData[i * _width * 4 + j * 4 + 1] = (GLubyte)(p1234.g * 255);
						newData[i * _width * 4 + j * 4 + 2] = (GLubyte)(p1234.b * 255);
						newData[i * _width * 4 + j * 4 + 3] = (GLubyte)(p1234.delta * 255);
					}
				}

				//Delete the old data
				delete[] _image->data;
	
				//Set the new data
				_image->data = newData;
	
				//Set the new width and height
				_image->width = _width;
				_image->height = _height;

				this->combineLayers();
			}
		}
	}
}

RGB ImageWork::getPixel(int _x, int _y) {
	RGB pixel = RGB(0, 0, 0, 0);

	if (_x >= 0 && _x < this->CurrentImage.width && _y >= 0 && _y < this->CurrentImage.height) {
		pixel.r = this->CurrentImage.data[_y * this->CurrentImage.width * 4 + _x * 4 + 0] / 255.0f;
		pixel.g = this->CurrentImage.data[_y * this->CurrentImage.width * 4 + _x * 4 + 1] / 255.0f;
		pixel.b = this->CurrentImage.data[_y * this->CurrentImage.width * 4 + _x * 4 + 2] / 255.0f;
		pixel.delta = this->CurrentImage.data[_y * this->CurrentImage.width * 4 + _x * 4 + 3] / 255.0f;
	}

	return pixel;
}

RGB ImageWork::getPixel(ImageStr* _image, int _x, int _y) {
	RGB pixel = RGB(0, 0, 0, 0);

	if (_x >= 0 && _x < _image->width && _y >= 0 && _y < _image->height) {
		pixel.r = _image->data[_y * _image->width * 4 + _x * 4 + 0] / 255.0f;
		pixel.g = _image->data[_y * _image->width * 4 + _x * 4 + 1] / 255.0f;
		pixel.b = _image->data[_y * _image->width * 4 + _x * 4 + 2] / 255.0f;
		pixel.delta = _image->data[_y * _image->width * 4 + _x * 4 + 3] / 255.0f;
	}

	return pixel;
}

void ImageWork::setPixel(int _x, int _y, RGB _color) {
	//Check if the pixel is in the image
	if (_x >= 0 && _x < this->CurrentImage.width && _y >= 0 && _y < this->CurrentImage.height) {
		int index = (_y * this->CurrentImage.width + _x) * this->CurrentImage.channels;
		CurrentImage.data[index] = static_cast<char>(_color.r * 255);
		CurrentImage.data[index + 1] = static_cast<char>(_color.g * 255);
		CurrentImage.data[index + 2] = static_cast<char>(_color.b * 255);
		if (CurrentImage.channels == 4) {
			CurrentImage.data[index + 3] = static_cast<char>(_color.delta * 255);
		}
	}
}

void ImageWork::setPixel(ImageStr* _image, int _x, int _y, RGB _color) {
	//Check if the pixel is in the image
	if (_x >= 0 && _x < _image->width && _y >= 0 && _y < _image->height) {
		int index = (_y * _image->width + _x) * _image->channels;
		_image->data[index] = static_cast<char>(_color.r * 255);
		_image->data[index + 1] = static_cast<char>(_color.g * 255);
		_image->data[index + 2] = static_cast<char>(_color.b * 255);
		if (_image->channels == 4) {
			_image->data[index + 3] = static_cast<char>(_color.delta * 255);
		}
	}
}