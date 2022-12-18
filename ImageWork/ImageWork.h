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
#include <queue>
#include <stack>
#include <omp.h>

#include <GLFW/glfw3.h>

struct RGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char delta;
};

//RGB Operator
//==
inline bool operator==(RGB a, RGB b) {
	return (a.r == b.r && a.g == b.g && a.b == b.b);
}

const struct ImageStr {
	std::string imagePath = "";
	std::string extension = "";
	std::string name = "";
	
	int width = 0;
	int height = 0;
	int channels = 0;
	
	GLubyte* data = NULL;
	GLuint texture = 0;
	int* histogramR = NULL;
	int histogramRSize = 0;
	int* histogramG = NULL;
	int histogramGSize = 0;
	int* histogramB = NULL;
	int histogramBSize = 0;
	
	bool modified = false;
	bool loaded = false;
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

enum Cursors
{
	CURSOR_AIM
};

class ImageWork {

	private:

		std::vector<ImageStr> Images;
		ImageStr CurrentImage;
		
		std::pair<int, int> CavasSize;

		RGB toolColor;
		RGB secondaryColor;
		int toolradius;
		int Tolerance;

		//Mouse points for interpolation (x,y)
		std::vector<std::pair<int, int>> mousePoints;

		//Min and Max of the selection rectangle
		std::pair<int, int> firstPoint;
		std::pair<int, int> selectionMin;
		std::pair<int, int> selectionMax;

		//Min and Max of the scale rectangle
		std::pair<int, int> firstScalePoint;
		std::pair<int, int> selectionScaleMin;
		std::pair<int, int> selectionScaleMax;
		
		//Selection enable
		bool selectionEnable;
		bool selectionDone;

		//Selection Scale enable
		bool selectionScaleEnable;
		bool selectionScaleDone;

		//Min and Max values for updating the image during interpolation
		std::pair<int, int> interpolationMin;
		std::pair<int, int> interpolationMax;

		void toolPencil(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolBrush(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolEraser(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolColorPicker(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolSelection(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolScale(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolBucket(int _MouseImagePositionX, int _MouseImagePositionY, int _tolerance);
		
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

		std::pair<int, int> getSelectionScaleMin();
		std::pair<int, int> getSelectionScaleMax();

		void setSelectionScaleMin(std::pair<int, int> _selectionScaleMin);
		void setSelectionScaleMax(std::pair<int, int> _selectionScaleMax);
		
		void clearMousePoints();
		void clearMousePairs();
		void clearFirstPointSelection();
		void clearFirstPointSelectionScale();
		void clearSelection();
		void clearImages();
		void selectionNormalize();
		void selectionScaleNormalize();

		void PushNewImage(ImageStr _Image);
		int ImagesSize();
		ImageStr* getImage(int _index);
		int getSizeImages();
		
		void swapImages(int _indexa, int _indexb);
		ImageStr* getImageStrP();
		
		bool useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY);
		void setToolColor(RGB _color);
		void setSecondaryColor(RGB _color);
		void setToolRadius(int _radius);
		
		void setSelectionEnable(bool _enable);
		void setSelectionScaleEnable(bool _enable);
		void setSelectionDone(bool _done);
		void setSelectionScaleDone(bool _done);

		void setTolerance(int _Tolerance);
		
		RGB getToolColor();
		RGB getSecondaryColor();
		int getToolRadius();
		
		bool getSelectionEnabled();
		bool getSelectionScaleEnabled();
		bool getSelectionDone();
		bool getSelectionScaleDone();
		
		int getTolerance();

		void reCopyImage(ImageStr* _Image);
		void resizeImage(ImageStr* _image, int _width, int _height);
		
		RGB getPixel(int _x, int _y);
		RGB getPixel(ImageStr* _image, int _x, int _y);
		void setPixel(int _x, int _y, RGB _color);
		void setPixel(ImageStr* _image, int _x, int _y, RGB _color);
		
		//Algorithms
		//floodfill
		void floodFill(int x, int y, RGB targetColor, RGB replacementColor, int tolerance);
		//Histogram
		void calculateHistogram(ImageStr* _Image);
};