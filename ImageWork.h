#pragma once

#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>

#include <GLFW/glfw3.h>

struct RGB {
	float r;
	float g;
	float b;
	float delta;
};

struct ImageStr {
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

		void toolPencil(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolBrush(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolEraser(int _MouseImagePositionX, int _MouseImagePositionY);
		
	public:
		
		ImageWork();
		~ImageWork();
		void init(std::vector<ImageStr>** _Images, ImageStr** _CurrentImage);

		void selectFrontImage();
		void selectImage(int _index);
		void swapImage(int _indexa, int _indexb);
		
		bool useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY);
		void setToolColor(RGB _color);
		RGB getToolColor();
};