#include "../ImageWork.h"

void ImageWork::toolPencil(int mx, int my, int r) {
	mousePoints.push_back({ mx, my });

	updateDirtyRect(mx, my, r);

	if (mousePoints.size() == 1) {
		fastStroke(mx, my, mx, my, r, BRUSH_HARD);
	} else {
		int px = mousePoints[mousePoints.size() - 2].first;
		int py = mousePoints[mousePoints.size() - 2].second;
		updateDirtyRect(px, py, r);
		fastStroke(px, py, mx, my, r, BRUSH_HARD);
	}

	// Keep only last 2 points — Bresenham only ever uses them
	if (mousePoints.size() > 2) {
		auto a = mousePoints[mousePoints.size() - 2];
		auto b = mousePoints[mousePoints.size() - 1];
		mousePoints.clear();
		mousePoints.push_back(a);
		mousePoints.push_back(b);
	}

	clampDirtyRect();
}
