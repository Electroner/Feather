#pragma once

#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <assert.h>

#include <GLFW/glfw3.h>

struct RGB {
	float r;
	float g;
	float b;
	float delta;
};

const struct ImageStr {
	std::string imagePath;
	std::string extension;
	std::string name;
	int width;
	int height;
	int channels;
	GLubyte* data;
	GLuint texture;
	bool loaded;
};

class ImageWork {

	private:

		std::vector<ImageStr> Images;
		ImageStr CurrentImage;
		RGB toolColor;
		RGB secondaryColor;
		int toolradius;

		//Mouse points for interpolation (x,y)
		std::vector<std::pair<int, int>> mousePoints;

		//Min and Max of the selection rectangle
		std::pair<int, int> firstPoint;
		std::pair<int, int> selectionMin;
		std::pair<int, int> selectionMax;

		//Selection enable
		bool selectionEnable;

		//Min and Max values for updating the image during interpolation
		std::pair<int, int> interpolationMin;
		std::pair<int, int> interpolationMax;

		void toolPencil(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolBrush(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolEraser(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolSelection(int _MouseImagePositionX, int _MouseImagePositionY);
		
	public:
		
		void init(std::vector<ImageStr>** _Images, ImageStr** _CurrentImage);

		std::pair<int, int> getInterpolationMin();
		std::pair<int, int> getInterpolationMax();

		std::pair<int, int> getSelectionMin();
		std::pair<int, int> getSelectionMax();
		
		void clearMousePoints();
		void clearMousePairs();
		void clearFirstPointSelection();
		void clearSelection();

		void selectFrontImage();
		void selectImage(int _index);
		void swapImage(int _indexa, int _indexb);
		
		bool useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY);
		void setToolColor(RGB _color);
		void setSecondaryColor(RGB _color);
		void setToolRadius(int _radius);
		void setSelectionEnable(bool _enable);
		RGB getToolColor();
		RGB getSecondaryColor();
		int getToolRadius();
		bool getSelectionEnabled();
				
		void setPixel(int _x, int _y, RGB _color);
};