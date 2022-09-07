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
	unsigned char* data;
	GLuint texture;
	bool loaded;
};

class ImageWork {

	private:

		std::vector<ImageStr> Images;
		ImageStr CurrentImage;

	public:
		
		ImageWork();
		~ImageWork();
		void init(std::vector<ImageStr>** _Images, ImageStr** _CurrentImage);
};