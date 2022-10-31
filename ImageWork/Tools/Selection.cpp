#include "../ImageWork.h"

void ImageWork::toolSelection(int _MouseImagePositionX, int _MouseImagePositionY) {
	//Enable selection
	this->setSelectionEnable(true);

	//Make the first point of the selection a corner of the rectangle selection
	//IF the first point are -1 set them to the current mouse position
	if (this->firstPoint.first == -1 && this->firstPoint.second == -1) {
		this->firstPoint.first = _MouseImagePositionX;
		this->firstPoint.second = _MouseImagePositionY;
		std::cout << "First point of selection set to: " << this->firstPoint.first << " " << this->firstPoint.second << std::endl;
	}
	
	//If the current point is less than the first point set the min to the current point and the max to the first
	if (_MouseImagePositionX < this->firstPoint.first) {
		this->selectionMin.first = _MouseImagePositionX;
		this->selectionMax.first = this->firstPoint.first;
	}
	if (_MouseImagePositionY < this->firstPoint.second) {
		this->selectionMin.second = _MouseImagePositionY;
		this->selectionMax.second = this->firstPoint.second;
	}
	
	//If the current point is bigger than the first point set the max to the current point and the min to the second
	if (_MouseImagePositionX > this->firstPoint.first) {
		this->selectionMax.first = _MouseImagePositionX;
		this->selectionMin.first = this->firstPoint.first;
	}
	if (_MouseImagePositionY > this->firstPoint.second) {
		this->selectionMax.second = _MouseImagePositionY;
		this->selectionMin.second = this->firstPoint.second;
	}
}