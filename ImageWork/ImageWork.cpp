#include "./ImageWork.h"

void ImageWork::init() {
	
	//set to false every loaded image in the vector
	for (int i = 0; i < this->Images.size(); i++) {
		Images.at(i).loaded = false;
	}

	this->CavasSize = std::make_pair(-1, -1);
	this->toolradius = 5;

	this->interpolationMin = std::pair<int, int>(this->CurrentImage.width, this->CurrentImage.height);
	this->interpolationMax = std::pair<int, int>(0, 0);

	this->selectionMin = std::pair<int, int>(0, 0);
	this->selectionMax = std::pair<int, int>(0, 0);

	this->selectionEnable = false;
	this->selectionDone = false;
	this->textEnable = false;
	
	//Colors
	this->toolColor = RGB(0, 0, 0, 255);
	this->secondaryColor = RGB(255, 255, 255, 255);

	//First point of the selection to -1
	this->firstPoint = std::pair<int, int>(-1, -1);
}

void ImageWork::addImage(ImageStr& _image) {
	//For the image create a Opengl texture
	if (_image.data != NULL) {
		//Enable transparency
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//Create the texture
		glGenTextures(1, &_image.texture);
		glBindTexture(GL_TEXTURE_2D, _image.texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (_image.channels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _image.width, _image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image.data);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, _image.data);
		}
		else if (_image.channels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _image.width, _image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.data);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, _image.data);
		}
		//Set the image as loaded
		_image.loaded = true;

		//Add the image to the vector
		this->Images.push_back(_image);
	}
}

void ImageWork::combineLayers() {
	//Set the current image to the first image in the vector
	if (Images.size() > 0) {
		this->CurrentImage = this->Images.front();
	}
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

void ImageWork::selectionNormalize() {
	if (this->selectionMin.first > this->selectionMax.first) {
		std::swap(this->selectionMin.first, this->selectionMax.first);
	}
	if (this->selectionMin.second > this->selectionMax.second) {
		std::swap(this->selectionMin.second, this->selectionMax.second);
	}

	//If the selection is out of the image, set it to the image size
	if (this->selectionMin.first < 0) {
		this->selectionMin.first = 0;
	}
	if (this->selectionMin.second < 0) {
		this->selectionMin.second = 0;
	}
	if (this->selectionMax.first > this->CurrentImage.width) {
		this->selectionMax.first = this->CurrentImage.width;
	}
	if (this->selectionMax.second > this->CurrentImage.height) {
		this->selectionMax.second = this->CurrentImage.height;
	}
}

void ImageWork::deleteSelection() {
	//if the selection is done and the selection is enabled the set the selected pixels to the secondary color
	if (this->selectionDone && this->selectionEnable) {
		for (int i = this->selectionMin.first; i < this->selectionMax.first; i++) {
			for (int j = this->selectionMin.second; j < this->selectionMax.second; j++) {
				this->setPixel(i, j, this->secondaryColor);
			}
		}
	}
}

void ImageWork::PushNewImage(ImageStr _Image) {
	this->Images.push_back(_Image);
}

int ImageWork::ImagesSize() {
	return (int)this->Images.size();
}

ImageStr* ImageWork::getImage(int _index) {
	return &this->Images.at(_index);
}

int ImageWork::getSizeImages() {
	return (int)this->Images.size();
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
	case TOOL_PENCIL:
		toolPencil(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case TOOL_BRUSH:
		toolBrush(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case TOOL_ERASER:
		toolEraser(_MouseImagePositionX, _MouseImagePositionY, this->toolradius);
		break;
	case TOOL_COLORPICKER:
		toolColorPicker(_MouseImagePositionX, _MouseImagePositionY);
		break;
	case TOOL_SELECTION:
		toolSelection(_MouseImagePositionX, _MouseImagePositionY);
		break;
	case TOOL_BUCKET:
		toolBucket(_MouseImagePositionX, _MouseImagePositionY, this->Tolerance);
		break;
	case TOOL_TEXT:
		toolText(_MouseImagePositionX, _MouseImagePositionY);
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

void ImageWork::setTolerance(int _tolerance) {
	this->Tolerance = _tolerance;
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

int ImageWork::getTolerance() {
	return this->Tolerance;
}

bool ImageWork::getTextEnabled() {
	return this->textEnable;
}

void ImageWork::setTextEnabled(bool _enable) {
	this->textEnable = _enable;
}

std::pair<int, int> ImageWork::getTextPosition() {
	return this->textPosition;
}

void ImageWork::setTextPosition(std::pair<int, int> _textPosition) {
	this->textPosition = _textPosition;
}

std::string ImageWork::getText() {
	return this->text;
}

void ImageWork::setText(std::string _text) {
	this->text = _text;
}

RGB ImageWork::getPixel(int _x, int _y) {
	RGB pixel = RGB(0, 0, 0, 0);

	if (_x >= 0 && _x < this->getImageStrP()->width && _y >= 0 && _y < this->getImageStrP()->height) {
		pixel.r = this->getImageStrP()->data[_y * this->getImageStrP()->width * this->getImageStrP()->channels + _x * this->getImageStrP()->channels + 0];
		pixel.g = this->getImageStrP()->data[_y * this->getImageStrP()->width * this->getImageStrP()->channels + _x * this->getImageStrP()->channels + 1];
		pixel.b = this->getImageStrP()->data[_y * this->getImageStrP()->width * this->getImageStrP()->channels + _x * this->getImageStrP()->channels + 2];
		if (this->getImageStrP()->channels == 4) {
			pixel.delta = this->getImageStrP()->data[_y * this->getImageStrP()->width * 4 + _x * 4 + 3];
		}
	}

	return pixel;
}

RGB ImageWork::getPixel(ImageStr* _image, int _x, int _y) {
	RGB pixel = RGB(0, 0, 0, 0);

	if (_x >= 0 && _x < _image->width && _y >= 0 && _y < _image->height) {
		pixel.r = _image->data[_y * _image->width * _image->channels + _x * _image->channels + 0];
		pixel.g = _image->data[_y * _image->width * _image->channels + _x * _image->channels + 1];
		pixel.b = _image->data[_y * _image->width * _image->channels + _x * _image->channels + 2];
		if (_image->channels == 4) {
			pixel.delta = _image->data[_y * _image->width * 4 + _x * 4 + 3];
		}
	}

	return pixel;
}

void ImageWork::setPixel(int _x, int _y, RGB _color) {
	//Check if the pixel is in the image
	if (_x >= 0 && _x < this->CurrentImage.width && _y >= 0 && _y < this->CurrentImage.height) {
		int index = (_y * this->CurrentImage.width + _x) * this->CurrentImage.channels;
		CurrentImage.data[index] =_color.r;
		CurrentImage.data[index + 1] = _color.g;
		CurrentImage.data[index + 2] = _color.b;
		if (CurrentImage.channels == 4) {
			CurrentImage.data[index + 3] = _color.delta ;
		}
	}
}

void ImageWork::setPixel(ImageStr* _image, int _x, int _y, RGB _color) {
	//Check if the pixel is in the image
	if (_x >= 0 && _x < _image->width && _y >= 0 && _y < _image->height) {
		int index = (_y * _image->width + _x) * _image->channels;
		_image->data[index] = _color.r;
		_image->data[index + 1] = _color.g;
		_image->data[index + 2] = _color.b;
		if (_image->channels == 4) {
			_image->data[index + 3] = _color.delta;
		}
	}
}

void ImageWork::reCopyImage(ImageStr* _Image) {
	this->Images.front() = *_Image;
}

void ImageWork::blackAndWhite(ImageStr* _Image) {
	RGB aux;
	for (int i = 0; i < _Image->width; i++) {
		for (int j = 0; j < _Image->height; j++) {
			aux = getPixel(_Image, i, j);
			unsigned char average = (aux.r + aux.g + aux.b) / 3;
			setPixel(_Image, i, j, RGB(average,average,average,aux.delta));
		}
	}
}

void ImageWork::negative(ImageStr* _Image) {
	RGB aux;
	for (int i = 0; i < _Image->width; i++) {
		for (int j = 0; j < _Image->height; j++) {
			aux = getPixel(_Image, i, j);
			setPixel(_Image, i, j, RGB(255 - aux.r, 255 - aux.g, 255 - aux.b, aux.delta));
		}
	}
}

void ImageWork::sepia(ImageStr* _Image) {
	RGB aux;
	for (int i = 0; i < _Image->width; i++) {
		for (int j = 0; j < _Image->height; j++) {
			aux = getPixel(_Image, i, j);
			unsigned char average = (aux.r + aux.g + aux.b) / 3;
			setPixel(_Image, i, j, RGB((unsigned char)(average * 0.9), (unsigned char)(average * 0.7), (unsigned char)(average * 0.5), aux.delta));
		}
	}
}
