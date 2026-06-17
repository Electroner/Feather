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
#include <cmath>
#include <cstring>

#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct RGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char delta;
};

//RGB Operator ==
inline bool operator==(RGB a, RGB b) {
	return (a.r == b.r && a.g == b.g && a.b == b.b);
}

struct ImageStr {
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
	bool visible = true;
	float opacity = 1.0f;

	// Text layer metadata (only meaningful when isTextLayer == true)
	bool isTextLayer = false;
	std::string textContent = "";
	int textFontSize = 24;
	int textAnchorX = 0;
	int textAnchorY = 0;
	int textRenderW = 0;
	int textRenderH = 0;
	RGB textColor = {0, 0, 0, 255};
};

// Text rendering utility — defined in Tools/Text.cpp
ImageStr rasterText(const char* text, const char* fontPath, int size, const RGB& color);

enum BrushMode { BRUSH_HARD, BRUSH_SOFT, BRUSH_ERASE };

enum Tools
{
	TOOL_PENCIL,
	TOOL_BRUSH,
	TOOL_ERASER,
	TOOL_COLORPICKER,
	TOOL_SELECTION,
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
		ImageStr CompositeImage;

		int activeLayerIndex;

		int canvasWidth;
		int canvasHeight;

		// ---- Undo / Redo ----
		struct UndoLayerData {
			std::string name;
			std::string extension;
			int width, height, channels;
			std::vector<unsigned char> data;
			bool visible;
			float opacity;
		};
		struct UndoState {
			std::vector<UndoLayerData> layers;
			int activeLayerIndex;
			int canvasWidth, canvasHeight;
		};
		static const int MAX_UNDO = 30;
		std::vector<UndoState> undoStack;
		std::vector<UndoState> redoStack;

		void buildUndoState(UndoState& out) const;

		// ---- Performance helpers ----
		void fastStroke(int x1, int y1, int x2, int y2, int radius, BrushMode mode);
		void resetInterpolationBounds();
		void updateDirtyRect(int x, int y, int r);
		void clampDirtyRect();

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

		//TEXT
		std::string text;
		std::pair<int, int> textPosition;
		bool textEnable;

	public:

		//FONTS AND TEXT
		std::string fontsPath;

		void init();

		void addImage(ImageStr &_image);

		// Layer compositing
		void compositeAllLayers();
		void compositeRegion(int x0, int y0, int rw, int rh);
		void combineLayers();

		// Active layer management
		void setActiveLayer(int index);
		int getActiveLayerIndex();
		ImageStr* getCompositeImage();

		// Undo / Redo
		void pushUndoState();
		void undo();
		void redo();
		bool canUndo() const;
		bool canRedo() const;

		// Canvas
		int  getCanvasWidth();
		int  getCanvasHeight();
		void setCanvasSize(int w, int h);
		void resizeCanvas(int newW, int newH, int anchorX, int anchorY);

		// Text layer operations
		void createTextLayer(const std::string& text, int x, int y, int fontSize, const RGB& color);
		void renderTextToLayer(int layerIdx);
		int  findTextLayerAt(int x, int y) const;

		// Layer operations
		void addNewLayer(const std::string& name);
		void insertImageOnTop(ImageStr _image);
		void removeLayer(int index);
		void duplicateLayer(int index);
		void mergeLayerDown(int index);
		void flattenAllLayers();

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
		void selectionNormalize();

		void deleteSelection();

		void PushNewImage(ImageStr _Image);
		int ImagesSize();
		ImageStr* getImage(int _index);
		int getSizeImages();

		void swapImages(int _indexa, int _indexb);
		void moveLayer(int from, int to);
		ImageStr* getImageStrP();

		bool useTool(int _tool, int _MouseImagePositionX, int _MouseImagePositionY);
		void setToolColor(RGB _color);
		void setSecondaryColor(RGB _color);
		void setToolRadius(int _radius);

		void setSelectionEnable(bool _enable);
		void setSelectionDone(bool _done);

		void setTolerance(int _Tolerance);

		RGB getToolColor();
		RGB getSecondaryColor();
		int getToolRadius();

		bool getSelectionEnabled();
		bool getSelectionDone();

		int getTolerance();

		bool getTextEnabled();
		void setTextEnabled(bool _enable);
		std::pair<int, int> getTextPosition();
		void setTextPosition(std::pair<int, int> _textPosition);
		std::string getText();
		void setText(std::string _text);

		RGB getPixel(int _x, int _y);
		RGB getPixel(ImageStr* _image, int _x, int _y);
		void setPixel(int _x, int _y, RGB _color);
		void setPixel(ImageStr* _image, int _x, int _y, RGB _color);

		void reCopyImage(ImageStr* _Image);

		//Tools
		void toolPencil(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolBrush(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolEraser(int _MouseImagePositionX, int _MouseImagePositionY, int _radius);
		void toolColorPicker(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolSelection(int _MouseImagePositionX, int _MouseImagePositionY);
		void toolBucket(int _MouseImagePositionX, int _MouseImagePositionY, int _tolerance);
		void toolText(int _MouseImagePositionX, int _MouseImagePositionY);

		//Algorithms
		void resizeImage(ImageStr* _image, int _width, int _height);
		void floodFill(int x, int y, RGB targetColor, RGB replacementColor, int tolerance);
		void calculateHistogram(ImageStr* _Image);
		void blackAndWhite(ImageStr* _Image);
		void negative(ImageStr* _Image);
		void sepia(ImageStr* _Image);
};
