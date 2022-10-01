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
		RGB toolcolor;

		//Mouse points for interpolation (x,y)
		std::vector<std::pair<int, int>> mousePoints;

		//Min and Max values for updating the image during interpolation
		std::pair<int, int> interpolationMin;
		std::pair<int, int> interpolationMax;

		void toolPencil(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolBrush(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolEraser(int _MouseImagePositionX, int _MouseImagePositionY);
		
	public:
		
		void init(std::vector<ImageStr>** _Images, ImageStr** _CurrentImage);

		std::pair<int, int> getInterpolationMin();
		std::pair<int, int> getInterpolationMax();
		
		void clearMousePoints();
		void clearMousePairs();

		void selectFrontImage();
		void selectImage(int _index);
		void swapImage(int _indexa, int _indexb);
		
		bool useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY);
		void setToolColor(RGB _color);
		RGB getToolColor();
};