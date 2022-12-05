#include "../ImageWork.h"

inline bool isSimilar(RGB colorA, RGB colorB, int tolerance)
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
	if (!isSimilar(this->getPixel(x, y),targetColor, tolerance)) {
		return;
	}
		
	//Create a queue of pixels
	std::queue<std::pair<int, int>> pixels;
	//Add the pixel to the queue
	pixels.push(std::make_pair(x, y));
	//While the queue is not empty
	while (!pixels.empty()) {
		//Get the pixel from the queue
		std::pair<int, int> pixel = pixels.front();
		//Remove the pixel from the queue
		pixels.pop();
		//Get the color of the pixel
		RGB color = this->getPixel(pixel.first, pixel.second);
		//Check if the color is the same as the target color
		if (color == targetColor) {
			//Set the color of the pixel
			this->setPixel(pixel.first, pixel.second, replacementColor);
			//Add the pixel to the left of the queue
			pixels.push(std::make_pair(pixel.first - 1, pixel.second));
			//Add the pixel to the right of the queue
			pixels.push(std::make_pair(pixel.first + 1, pixel.second));
			//Add the pixel above the queue
			pixels.push(std::make_pair(pixel.first, pixel.second - 1));
			//Add the pixel below the queue
			pixels.push(std::make_pair(pixel.first, pixel.second + 1));
		}
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