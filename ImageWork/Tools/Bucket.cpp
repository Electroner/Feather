#include "../ImageWork.h"
#include <Windows.h>
bool isSimilar(RGB colorA, RGB colorB, int tolerance)
{
	return  (abs(colorA.r - colorB.r) <= tolerance) &&
		(abs(colorA.g - colorB.g) <= tolerance) &&
		(abs(colorA.b - colorB.b) <= tolerance);
}

//FloodFill Function
void ImageWork::floodFill(int x, int y, RGB targetColor, RGB replacementColor, int tolerance) {
	//Check if the target color is the same as the replacement color
	if (targetColor == replacementColor) {
		return;
	}
	//Check if the target color is the same as the color at the pixel with the tolerance
	if (!isSimilar(this->getPixel(x, y), targetColor, tolerance)) {
		return;
	}

	//Create a queue of pixels
	std::queue<std::pair<int, int>> queue;
	std::pair<int, int> pixel;

	ImageStr* image = this->getImageStrP();

	//Add the pixel to the queue
	queue.push(std::make_pair(x, y));
	
	//While the queue is not empty
	while (!queue.empty()) {
		pixel = queue.front();
		
		//If the pixel color is the same as the target color
		if (this->getPixel(image,pixel.first, pixel.second) == targetColor) {
			//Set the pixel color to the replacement color
			this->setPixel(image,pixel.first, pixel.second, replacementColor);

			//Add the pixel's neighbors to the queue
			if (pixel.first < image->width - 1) {
				queue.push(std::make_pair(pixel.first + 1, pixel.second));
			}
			if (pixel.first > 0) {
				queue.push(std::make_pair(pixel.first - 1, pixel.second));
			}
			if (pixel.second > 0) {
				queue.push(std::make_pair(pixel.first, pixel.second - 1));
			}
			if (pixel.second < image->height - 1) {
				queue.push(std::make_pair(pixel.first, pixel.second + 1));
			}
		}
		
		queue.pop();
	}
}


void ImageWork::toolBucket(int _MouseImagePositionX, int _MouseImagePositionY, int _tolerance) {
	//Bucket tool
	//Check if the mouse is inside the image
	if (_MouseImagePositionX >= 0 && _MouseImagePositionX < CurrentImage.width && _MouseImagePositionY >= 0 && _MouseImagePositionY < CurrentImage.height) {
		//Flood Fill Algoritm
		floodFill(_MouseImagePositionX, _MouseImagePositionY, this->getPixel(_MouseImagePositionX, _MouseImagePositionY), this->toolColor, _tolerance);
	}
}