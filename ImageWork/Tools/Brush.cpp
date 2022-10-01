#include "../ImageWork.h"

void ImageWork::toolBrush(int _MouseImagePositionX, int _MouseImagePositionY, int _radius) {	
	//Add point to mousePoints
	mousePoints.push_back(std::make_pair(_MouseImagePositionX, _MouseImagePositionY));

	//Update interpolation min and max
	if (this->interpolationMin.first > _MouseImagePositionX) {
		this->interpolationMin.first = _MouseImagePositionX - _radius;
	}
	if (this->interpolationMin.second > _MouseImagePositionY) {
		this->interpolationMin.second = _MouseImagePositionY - _radius;
	}
	if (this->interpolationMax.first < _MouseImagePositionX) {
		this->interpolationMax.first = _MouseImagePositionX + _radius;
	}
	if (this->interpolationMax.second < _MouseImagePositionY) {
		this->interpolationMax.second = _MouseImagePositionY + _radius;
	}
	
	//If the size is 11 or more clear the vector and let the last two points be the first points for optimization
	if (mousePoints.size() >= 11) {
		std::pair<int, int> lastPoint = mousePoints[mousePoints.size() - 1];
		std::pair<int, int> secondLastPoint = mousePoints[mousePoints.size() - 2];
		mousePoints.clear();
		mousePoints.push_back(secondLastPoint);
		mousePoints.push_back(lastPoint);

		//Calculate the new interpolation min and max values with the last two points
		if (lastPoint.first < secondLastPoint.first) {
			this->interpolationMin.first = lastPoint.first - _radius;
			this->interpolationMax.first = secondLastPoint.first + _radius;
		}
		else {
			this->interpolationMin.first = secondLastPoint.first - _radius;
			this->interpolationMax.first = lastPoint.first + _radius;
		}
		if (lastPoint.second < secondLastPoint.second) {
			this->interpolationMin.second = lastPoint.second - _radius;
			this->interpolationMax.second = secondLastPoint.second + _radius;
		}
		else {
			this->interpolationMin.second = secondLastPoint.second - _radius;
			this->interpolationMax.second = lastPoint.second + _radius;
		}
	}

	//If there is only one point in the vector, draw a circle in the image
	if (mousePoints.size() == 1) {
		for (int x = -_radius; x <= _radius; x++) {
			for (int y = -_radius; y <= _radius; y++) {
				if (x * x + y * y <= _radius * _radius) {
					this->setPixel(_MouseImagePositionX + x, _MouseImagePositionY + y, this->toolcolor);
				}
			}
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
			for (int x = -_radius; x <= _radius; x++) {
				for (int y = -_radius; y <= _radius; y++) {
					if (x * x + y * y <= _radius * _radius) {
						this->setPixel(x1 + x, y1 + y, this->toolcolor);
					}
				}
			}
			if (x1 == x2 && y1 == y2) break;
			e2 = 2 * err;
			if (e2 > -dy) { err -= dy; x1 += sx; }
			if (e2 < dx) { err += dx; y1 += sy; }
		}
	}
}