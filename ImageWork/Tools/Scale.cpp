#include "../ImageWork.h"

void ImageWork::toolScale(int _MouseImagePositionX, int _MouseImagePositionY) {
	if (!this->selectionScaleDone) {
		//Enable selection
		this->setSelectionScaleEnable(true);

		//Make the first point of the selection a corner of the rectangle selection
		//IF the first point are -1 set them to the current mouse position
		if (this->firstScalePoint.first == -1 && this->firstScalePoint.second == -1) {
			this->firstScalePoint.first = _MouseImagePositionX;
			this->firstScalePoint.second = _MouseImagePositionY;
			std::cout << "First point of selection set to: " << this->firstScalePoint.first << " " << this->firstScalePoint.second << std::endl;
		}

		//If the current point is less than the first point set the min to the current point and the max to the first
		if (_MouseImagePositionX < this->firstScalePoint.first) {
			this->selectionScaleMin.first = _MouseImagePositionX;
			this->selectionScaleMax.first = this->firstScalePoint.first;
		}
		if (_MouseImagePositionY < this->firstScalePoint.second) {
			this->selectionScaleMin.second = _MouseImagePositionY;
			this->selectionScaleMax.second = this->firstScalePoint.second;
		}

		//If the current point is bigger than the first point set the max to the current point and the min to the second
		if (_MouseImagePositionX > this->firstScalePoint.first) {
			this->selectionScaleMax.first = _MouseImagePositionX;
			this->selectionScaleMin.first = this->firstScalePoint.first;
		}
		if (_MouseImagePositionY > this->firstScalePoint.second) {
			this->selectionScaleMax.second = _MouseImagePositionY;
			this->selectionScaleMin.second = this->firstScalePoint.second;
		}
	}
}