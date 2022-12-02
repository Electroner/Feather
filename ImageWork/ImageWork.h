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
	bool modified;
	bool loaded;
};

enum Tools
{
	TOOL_PENCIL,
	TOOL_BRUSH,
	TOOL_ERASER,
	TOOL_COLORPICKER,
	TOOL_SELECTION,
	TOOL_SCALE,
	TOOL_BUCKET,
	TOOL_TEXT
};

class ImageWork {

	private:

		std::vector<ImageStr> Images;
		ImageStr CurrentImage;
		
		std::pair<int, int> CavasSize;

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
		bool selectionDone;

		//Min and Max values for updating the image during interpolation
		std::pair<int, int> interpolationMin;
		std::pair<int, int> interpolationMax;

		void toolPencil(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolBrush(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolEraser(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolSelection(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolColorPicker(int _MouseImagePositionX, int _MouseImagePositionY);
		
	public:
		
		void init();

		void combineLayers();

		std::pair<int, int> getInterpolationMin();
		std::pair<int, int> getInterpolationMax();

		void setInterpolationMin(std::pair<int, int> _interpolationMin);
		void setInterpolationMax(std::pair<int, int> _interpolationMax);

		std::pair<int, int> getSelectionMin();
		std::pair<int, int> getSelectionMax();

		void setSelectionMin(std::pair<int, int> _selectionMin);
		void setSelectionMax(std::pair<int, int> _selectionMax);
		
		void clearMousePoints();
		void clearMousePairs();
		void clearFirstPointSelection();
		void clearSelection();
		void clearImages();

		void PushNewImage(ImageStr _Image);
		int ImagesSize();
		ImageStr* getImage(int _index);
		
		void swapImages(int _indexa, int _indexb);
		ImageStr* getImageStrP();
		
		bool useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY);
		void setToolColor(RGB _color);
		void setSecondaryColor(RGB _color);
		void setToolRadius(int _radius);
		void setSelectionEnable(bool _enable);
		void setSelectionDone(bool _done);
		void selectionNormalize();
		RGB getToolColor();
		RGB getSecondaryColor();
		int getToolRadius();
		bool getSelectionEnabled();
		bool getSelectionDone();

		void reCopyImage(ImageStr* _Image);
		void resizeImage(ImageStr* _image, int _width, int _height);
		
		RGB getPixel(int _x, int _y);
		RGB getPixel(ImageStr* _image, int _x, int _y);
		void setPixel(int _x, int _y, RGB _color);
		void setPixel(ImageStr* _image, int _x, int _y, RGB _color);
};