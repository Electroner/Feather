#include "ImageWork.h"

ImageWork::ImageWork() {}

ImageWork::~ImageWork() {}

void ImageWork::init(std::vector<ImageStr>** _Images, ImageStr** _CurrentImage) {
	*_Images = &this->Images;
	*_CurrentImage = &this->CurrentImage;
}